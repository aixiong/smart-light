#include"json\json.h"
#include<string>
#include "dataProcessor.h"
#include"dataImpl.h"

dataProcessor::dataProcessor()
{
	impl = std::make_shared<dataImpl>();
}

bool dataProcessor::getSwitch()
{
	std::string message = querySwitch();
	return parseSwitch(message);
}

void dataProcessor::putPower(int power)
{
	int cnt = 3;
	while (!putPowerInfo(power) && cnt)
	{
		_sleep(100);
		--cnt;
	}
}

std::string dataProcessor::querySwitch()
{
	return impl->querySwitch();
		;
}

bool dataProcessor::parseSwitch(const std::string & message)
{
	return impl->parseSwitch(message);
}

bool dataProcessor::putPowerInfo(int power)
{
	return impl->putPowerInfo(power);
}
