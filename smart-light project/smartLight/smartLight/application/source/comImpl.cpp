#include "..\include\comImpl.h"

void comImpl::initCom(const std::string & ipStr, unsigned portNum)
{
	initEnvironment();
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)throw "bad socket!";
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(ipStr.c_str());
	server_addr.sin_port = htons(portNum);
	connected = false;
}

bool comImpl::connectServer()
{
	if (connect(m_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		return false;
	}
	connected = true;
	return true;
}

unsigned comImpl::sendString(const std::string & str)
{
	return send(m_sock, str.c_str(), str.length(), 0);
}

std::string comImpl::recvString()
{
	char tmp[1024] = { 0 };
	unsigned cnt = recv(m_sock, tmp, 1024, 0);
	tmp[cnt] = 0;
	return std::string(tmp);
}

bool comImpl::initEnvironment()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA tmp;
	if (WSAStartup(sockVersion, &tmp) != 0)
	{
		return false;
	}
	return true;
}
