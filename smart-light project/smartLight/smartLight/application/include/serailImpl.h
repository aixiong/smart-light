#pragma once
#include<string>
#include<memory>

class _sync_com;

class serailImpl
{
public:
	serailImpl(int _comName, int _baudRate);
	~serailImpl();
	void open();
	void close();
	bool isOpen()const
	{
		return opened;
	}
	int sendString(const std::string& str);
	int recvString(std::string& str);
private:
	void openCheck();
	std::shared_ptr<_sync_com> impl;
	int comName;
	int baudRate;
	bool opened;
};