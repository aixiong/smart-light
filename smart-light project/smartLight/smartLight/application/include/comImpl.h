#pragma once
#include<WinSock2.h>
#include<string>
#include<exception>
#pragma comment(lib,"ws2_32.lib")

class comImpl
{
public:
	comImpl(const std::string& ipStr, unsigned portNum)
	{
		initCom(ipStr, portNum);
	}
	~comImpl()
	{
		connected = false;
		closesocket(m_sock);
		clearEnvironment();
	}
	void initCom(const std::string& ipStr, unsigned portNum);
	bool connectServer();
	unsigned sendString(const std::string& str);
	std::string recvString();
	bool isConnected() const
	{
		return connected;
	}
private:
	bool initEnvironment();
	void clearEnvironment()
	{
		WSACleanup();
	}
private:
	SOCKET m_sock;
	sockaddr_in server_addr;
	bool connected;
};