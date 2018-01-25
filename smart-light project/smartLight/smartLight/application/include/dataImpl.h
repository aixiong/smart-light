#pragma once
#include<string>
#include"communication.h"
#include"../../json/include/json.h"


class dataImpl
{
public:
	dataImpl(){}
	std::string querySwitch();
	bool parseSwitch(const std::string& message);
	bool putPowerInfo(int power);
	std::string getPowerInfo(int power);
private:
	std::string toStr(int num);
private:
	communication communicator;
	Json::Reader reader;
	Json::FastWriter writer;
};