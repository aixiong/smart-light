// smartLight.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<future>
#include"application\include\dataprocessor.h"
#include"application\include\serailport.h"
using namespace std;

bool status = false;

int main()
{
	dataProcessor tool;
	while (true)
	{
		if (status != tool.getSwitch())
		{
			status = !status;
			if (status)
			{
				cout << "turn on the light." << endl;
			}
			else
			{
				cout << "turn off the light." << endl;
			}
		}
		if (status)
		{
			tool.putPower(20);
		}
		else
		{
			tool.putPower(0);
		}
		_sleep(1000);
	}
	system("pause");
    return 0;
}

