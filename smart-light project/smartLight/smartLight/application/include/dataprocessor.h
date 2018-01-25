#pragma once
#include<memory>
#include<string>
class dataImpl;

class dataProcessor
{
private:
	std::shared_ptr<dataImpl> impl;
public:
	dataProcessor();
	bool getSwitch();
	void putPower(int power);
private:
	std::string querySwitch();
	bool parseSwitch(const std::string& message);
	bool putPowerInfo(int power);
};