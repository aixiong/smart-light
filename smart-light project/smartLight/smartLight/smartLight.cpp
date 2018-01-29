// smartLight.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<future>
#include"application\include\serailport.h"
using namespace std;
serailport one(4, 115200);
serailport two(3, 115200);

void send()
{
	string str;
	cin >> str;
	one.sendCommand(str);
}

void recv()
{
	string str;
	
	if(two.recvCommand(str))
	{
		printf("%s", str.data());
	}
}

int main()
{
	/*future<void> f1 = async(send);
	future<void> f2 = async(recv);*/
	while (true)
	{
		send();
		//recv();
		_sleep(100);
	}
	//system("pause");
    return 0;
}

