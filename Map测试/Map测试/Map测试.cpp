// Map����.cpp : �������̨Ӧ�ó������ڵ㡣
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
	cout << "Map����������" << Times << "�Σ�����" << GetTickCount() - Time << "����\n";
	
	system("pause");
    return 0;
}
/*
unsigned long Time=GetTickCount();

puts("*****************************\n��������ǣ�");
PrimeNums1(a,b);
printf("���β���ϵͳ������%d����\n",GetTickCount()-Time);
*/
