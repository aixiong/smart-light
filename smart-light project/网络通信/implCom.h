#pragma once
#include<WinSock2.h>
#include<string>
#include<exception>
#pragma comment(lib,"ws2_32.lib")

class implCom
{
public:
	implCom(const std::string& ipStr, unsigned portNum)
	{
		initEnvironment();
		m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_sock == INVALID_SOCKET)throw "bad socket!";
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.S_un.S_addr = inet_addr(ipStr.c_str());
		server_addr.sin_port = htons(portNum);
		connected = false;
	}
	~implCom()
	{
		connected = false;
		closesocket(m_sock);
		clearEnvironment();
	}
	bool connectServer()
	{
		if (connect(m_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
		{
			return false;
		}
		connected = true;
		return true;
	}
	unsigned sendString(const std::string& str)
	{
		return send(m_sock, str.c_str(), str.length(), 0);
	}
	std::string recvString()
	{
		char tmp[1024] = { 0 };
		unsigned cnt = recv(m_sock, tmp, 1024, 0);
		tmp[cnt] = 0;
		return std::string(tmp);
	}
	bool isConnected()
	{
		return connected;
	}
private:
	bool initEnvironment()
	{
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA tmp;
		if (WSAStartup(sockVersion, &tmp) != 0)
		{
			return false;
		}
		return true;
	}
	void clearEnvironment()
	{
		WSACleanup();
	}
private:
	SOCKET m_sock;
	sockaddr_in server_addr;
	bool connected;
};