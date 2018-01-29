#include"../include/communication.h"
#include"../include/comImpl.h"

communication::communication(std::string ipStr, unsigned portNum):impl(new comImpl(ipStr,portNum))
{
}

bool communication::connectServer()
{
	if(!impl->isConnected())
	return impl->connectServer();
}

bool communication::sendString(const std::string & content)
{
	return impl->sendString(content);
}

bool communication::recvString(std::string & content)
{
	content = impl->recvString();
	return !content.empty();
}
