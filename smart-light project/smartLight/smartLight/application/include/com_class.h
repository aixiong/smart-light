/*
This file is on implemention of com.
*/
#ifndef	_COM_CLASS_H_
#define _COM_CLASS_H_

#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)

#include <cassert>
#include <strstream>
#include <algorithm>
#include <exception>
#include <atlstr.h>
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>            // for String... functions
#include <iomanip>
using namespace std;

#include <windows.h>

class _base_com	  //虚基类 基本串口接口
{
public:

	volatile int _port;	 //串口号
	volatile HANDLE _com_handle;//串口句柄
	DCB _dcb;			  //波特率，停止位，等
	int _in_buf, _out_buf; // 缓冲区
	COMMTIMEOUTS _co;	 //	超时时间
	//虚函数,用于不同方式的串口打开
	virtual bool open_port() = 0;

	void init()	//初始化
	{
		memset(&_dcb, 0, sizeof(_dcb));
		_dcb.DCBlength = sizeof(_dcb);

		_com_handle = INVALID_HANDLE_VALUE;

		_in_buf = _out_buf = 8192;

		memset(&_co, 0, sizeof(_co));
		_co.ReadIntervalTimeout = 0xFFFFFFFF;
		_co.ReadTotalTimeoutMultiplier = 0;
		_co.ReadTotalTimeoutConstant = 0;
		_co.WriteTotalTimeoutMultiplier = 0;
		_co.WriteTotalTimeoutConstant = 5000;
	}

public:
	_base_com()
	{
		init();
	}
	virtual ~_base_com()
	{
		close();
	}
	/*基本参数设置*/
	//设置串口参数：波特率，停止位，等
	inline bool set_para()
	{
		return is_open() ? SetCommState(_com_handle, &_dcb) : false;
	}
	//打开设置对话框
	bool config_dialog()
	{
		if (is_open())
		{
			COMMCONFIG cf;
			memset(&cf, 0, sizeof(cf));
			cf.dwSize = sizeof(cf);
			cf.wVersion = 1;

			char com_str[10];
			strcpy(com_str, "COM");
			ltoa(_port, com_str + 3, 10);

			CString str = CString(com_str);

			if (CommConfigDialog(str, NULL, &cf))
			{
				memcpy(&_dcb, &cf.dcb, sizeof(DCB));
				return SetCommState(_com_handle, &_dcb);
			}
		}
		return false;
	}
	//支持设置字符串 "9600, 8, n, 1"
	bool set_dcb(char *set_str)
	{
		CString str = CString(set_str);
		return bool(BuildCommDCB(str, &_dcb));
	}
	//设置内置结构串口参数：波特率，停止位
	bool set_dcb(int BaudRate, int ByteSize = 8, int Parity = NOPARITY, int StopBits = ONESTOPBIT)
	{
		_dcb.BaudRate = BaudRate;
		_dcb.ByteSize = ByteSize;
		_dcb.Parity = Parity;
		_dcb.StopBits = StopBits;
		return true;
	}
	//设置缓冲区大小
	inline bool set_buf(int in_buf, int out_buf)
	{
		return is_open() ? SetupComm(_com_handle, in_buf, out_buf) : false;
	}
	//打开串口 缺省 9600, 8, n, 1
	inline bool open(int port)
	{
		if (port < 1 || port > 1024)
			return false;


		_port = port;

		return open_port();
	}
	//打开串口 缺省 baud_rate, 8, n, 1
	inline bool open(int port, int baud_rate)
	{
		if (port < 1 || port > 1024)
			return false;

		set_dcb(baud_rate);
		_port = port;

		return open_port();
	}
	//打开串口
	inline bool open(int port, char *set_str)
	{
		if (port < 1 || port > 1024)
			return false;

		CString str = CString(set_str);

		if (!BuildCommDCB(set_str, &_dcb))
			return false;

		_port = port;

		return open_port();
	}
	//关闭串口
	inline virtual void close()
	{
		if (is_open())
		{
			CloseHandle(_com_handle);
			_com_handle = INVALID_HANDLE_VALUE;
		}
	}
	//判断串口是或打开
	inline bool is_open()
	{
		return _com_handle != INVALID_HANDLE_VALUE;
	}
	//获得串口句炳
	HANDLE get_handle()
	{
		return _com_handle;
	}
};



class _sync_com : public _base_com
{
protected:
	//打开串口
	virtual bool open_port()
	{
		if (is_open())
			close();

		char com_str[10];

		strcpy(com_str, "COM");
		ltoa(_port, com_str + 3, 10);
		CString str = CString(com_str);
		
		_com_handle = CreateFile(
			str,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		assert(is_open());
		if (!is_open())//检测串口是否成功打开
			return false;

		BOOL ret;
		ret = SetupComm(_com_handle, _in_buf, _out_buf);	//设置推荐缓冲区
		assert(ret);
		ret &= SetCommState(_com_handle, &_dcb);	//设置串口参数：波特率，停止位，等
		assert(ret);
		ret &= SetCommTimeouts(_com_handle, &_co);	//设置超时时间
		assert(ret);
		ret &= PurgeComm(_com_handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);	//清空串口缓冲区
		assert(ret);
		if (!ret)
		{
			close();
			return false;
		}

		return true;
	}

public:

	_sync_com()
	{
		_co.ReadTotalTimeoutConstant = 5000;
	}
	//同步读
	int read(char *buf, int buf_len)
	{
		if (!is_open())
			return 0;

		buf[0] = '\0';

		COMSTAT  stat;
		DWORD error;

		if (ClearCommError(_com_handle, &error, &stat) && error > 0)	//清除错误
		{
			PurgeComm(_com_handle, PURGE_RXABORT | PURGE_RXCLEAR); /*清除输入缓冲区*/
			return 0;
		}

		unsigned long r_len = 0;

		buf_len = min(buf_len - 1, (int)stat.cbInQue);
		if (!ReadFile(_com_handle, buf, buf_len, &r_len, NULL))
			r_len = 0;
		//buf[r_len] = '\0';

		return r_len;
	}
	//同步写
	int write(const char *buf, int buf_len)
	{
		if (!is_open() || !buf)
			return 0;

		DWORD    error;
		if (ClearCommError(_com_handle, &error, NULL) && error > 0)	//清除错误
			PurgeComm(_com_handle, PURGE_TXABORT | PURGE_TXCLEAR);

		unsigned long w_len = 0;
		if (!WriteFile(_com_handle, buf, buf_len, &w_len, NULL))
			w_len = 0;

		return w_len;
	}
	//同步写
	inline int write(const char *buf)
	{
		assert(buf);
		return write(buf, strlen(buf));
	}
	//同步写, 支持部分类型的流输出
	template<typename T>
	_sync_com& operator << (T x)
	{
		strstream s;

		s << x;
		write(s.str(), s.pcount());

		return *this;
	}
};



#endif _COM_CLASS_H_