#pragma once
/*
string 为什么不能提前声明？
*/
#include<memory>
#include<string>

class serailImpl;

class serailport
{
public:
	//serailport(); //这个构造函数暂时搁置
	serailport(int comName, int baudRate = 115200);
	bool isOpen()const;
	int sendCommand(const std::string& command);
	int recvCommand(std::string& command);
private:
	std::shared_ptr<serailImpl> impl;
private:
	//std::string findCom(); //这个函数用于自动寻找可用串口
};