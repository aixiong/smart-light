#pragma once
/*
string Ϊʲô������ǰ������
*/
#include<memory>
#include<string>

class serailImpl;

class serailport
{
public:
	//serailport(); //������캯����ʱ����
	serailport(int comName, int baudRate = 115200);
	bool isOpen()const;
	int sendCommand(const std::string& command);
	int recvCommand(std::string& command);
private:
	std::shared_ptr<serailImpl> impl;
private:
	//std::string findCom(); //������������Զ�Ѱ�ҿ��ô���
};