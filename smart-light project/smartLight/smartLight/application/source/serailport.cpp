#include "..\include\serailport.h"
#include"..\include\serailImpl.h"

serailport::serailport(int comName, int baudRate)
{
	impl = std::make_shared<serailImpl>(comName, baudRate);
}

bool serailport::isOpen() const
{
	return impl->isOpen();
}

int serailport::sendCommand(const std::string & command)
{
	return impl->sendString(command);
}

int serailport::recvCommand(std::string & command)
{
	return impl->recvString(command);
}

