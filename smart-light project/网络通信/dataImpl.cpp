#include "dataImpl.h"

const static std::string postCommand = R"(POST /devices/769809/datapoints HTTP/1.1
api-key: cnVyDeqtsfKRDOhs7=4ZE2ppnXw=
Host: api.heclouds.com
Content-Length: )";
const static std::string getCommand = R"(GET /devices/769809/datapoints HTTP/1.1
api-key: cnVyDeqtsfKRDOhs7=4ZE2ppnXw=
Host: api.heclouds.com

)";
std::string dataImpl::querySwitch()
{
	if (!communicator.connectServer())
	{
		throw connectFailure();
	}
	communicator.sendString(getCommand);
	std::string message;
	communicator.recvString(message);
	auto pos = message.find("\r\n\r\n");
	message = message.substr(pos + 4);
	return message;
}

bool dataImpl::parseSwitch(const std::string & message)
{
	Json::Value root;
	reader.parse(message, root);
	Json::Value val = root["data"]["datastreams"];
	std::string ans = "";
	for (int i = 0; i < 2; ++i)
	{
		if (val[i]["id"] == "on")
		{
			ans = val[i]["datapoints"][0]["value"].asString();
			break;
		}
	}
	if (ans == "1")return true;
	else return false;
}

bool dataImpl::putPowerInfo(int power)
{
	std::string str = getPowerInfo(power);
	std::string message = postCommand;
	message += toStr(str.length());
	message += "\r\n\r\n";
	message += str;
	message += "\r\n";
	if (!communicator.connectServer())
	{
		throw connectFailure();
	}
	communicator.sendString(message);
	std::string tmp;
	communicator.recvString(tmp);
	return true;
}

std::string dataImpl::getPowerInfo(int power)
{
	Json::Value root;
	Json::Value data;
	Json::Value val;
	data["id"] = "power";
	char buff[24] = { 0 };
	val["value"] = toStr(power);
	data["datapoints"].append(val);
	root["datastreams"].append(data);
	std::string res = writer.write(root);
	return res;
}

std::string dataImpl::toStr(int num)
{
	char buff[32] = { 0 };
	_itoa_s(num, buff, 10);
	return std::string(buff);
}
