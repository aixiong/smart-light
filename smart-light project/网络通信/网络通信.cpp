// 网络通信.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include"dataProcessor.h"
using namespace std;






int main()
{
	dataProcessor data;
	data.getSwitch();
	data.putPower(100);
	system("pause");
    return 0;
}

