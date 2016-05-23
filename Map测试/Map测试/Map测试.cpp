// Map测试.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <map>
#include <iostream>
#include <list>
#include <Windows.h>
using namespace std;
const long long Times = 1000000;
int main()
{
	map<int, int> mymap;
	long long i = 0;
	while (i < Times) {
		mymap.insert({ i,i*i+48*i });
		++i;
	}
	cout << mymap.size() << endl;
	int j = 0;
	auto Time = GetTickCount();
	while (j < mymap.size()) 
		mymap.find(j++);
	cout << "Map二叉树搜索" << Times << "次，用了" << GetTickCount() - Time << "毫秒\n";
	
	system("pause");
    return 0;
}
/*
unsigned long Time=GetTickCount();

puts("*****************************\n搜索结果是：");
PrimeNums1(a,b);
printf("本次操作系统运算了%d毫秒\n",GetTickCount()-Time);
*/
