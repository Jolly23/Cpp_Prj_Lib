// 获取系统时间.cpp : 定义控制台应用程序的入口点。
// 精确到毫秒

#include "stdafx.h"
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <iostream>

using namespace std;
void Now(char* rBuff);
int yday2mday(int nDays, int nYear);

int main()
{
	char buff[64];
	Now(buff);
	cout << buff << endl;
	system("pause");
	return 0;
}

void Now(char *rBuff)
{
	struct timeb tp;
	struct tm *tm;
	ftime(&tp);
	tm = localtime(&tp.time);
	sprintf(rBuff, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d",
		1900 + tm->tm_year,
		1 + tm->tm_mon,
		yday2mday(tm->tm_yday, 1900 + tm->tm_year),
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec,
		tp.millitm);
}

int yday2mday(int nDays, int nYear)
{
	//天数自增1（1月1日为 0）
	nDays += 1;

	//判断平闰年，确定2月天数
	int nDaysOfFeb = 0;
	if (nYear % 4 == 0  &&  nYear % 100 != 0  ||  nYear % 400 == 0)
		nDaysOfFeb = 29;
	else
		nDaysOfFeb = 28;

	//返回年天数 - 月已过天数
	if (nDays <= 31)
		return nDays;
	else if (nDays <= 31 + nDaysOfFeb)
		return nDays - 31;
	else if (nDays <= 31 + nDaysOfFeb + 31)
		return nDays - 31 - nDaysOfFeb;
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30);
}