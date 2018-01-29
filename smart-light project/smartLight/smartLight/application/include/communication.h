#pragma once
#include<memory>
#include<string>

class comImpl;

class communication
{
public:
	communication(std::string ipStr="183.230.40.33",unsigned portNum=80);
	bool connectServer();
	bool sendString(const std::string& content);
	bool recvString(std::string& content);
private:
	std::shared_ptr<comImpl> impl;
};

struct connectFailure
{
};