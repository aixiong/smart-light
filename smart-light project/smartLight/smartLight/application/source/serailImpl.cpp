#include "..\include\serailImpl.h"
#include"..\include\com_class.h"

serailImpl::serailImpl(int _comName, int _baudRate) :impl(new _sync_com()), comName(_comName), baudRate(_baudRate), opened(false) {}

serailImpl::~serailImpl()
{
	if (isOpen())close();
}

void serailImpl::open()
{
	if (impl->is_open())return;
	impl->open(comName, baudRate);
	this->opened = true;
}

void serailImpl::close()
{
	if (impl->is_open())
	{
		impl->close();
		this->opened = false;
	}
}

int serailImpl::sendString(const std::string & str)
{
	openCheck();
	return impl->write(str.data());
}

int serailImpl::recvString(std::string & str)
{
	openCheck();
	char buff[128] = { 0 };
	int num=impl->read(buff,sizeof(buff)/sizeof(char)-1);
	str = buff;
	return num;
}

void serailImpl::openCheck()
{
	if (!opened)
	{
		impl->open(comName, baudRate);
		opened = true;
	}
}
