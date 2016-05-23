// 猜数字游戏.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <process.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
using namespace std;

struct UserData
{
	int SumGameTimes;
	int SumSteps;
	int SumFaultTimes;
}   g_UserData;

int  RandNumb();
void Game();
int  GameOS();
void SaveUserData();
void LoadUserData();
void UserAnalysis();

void main()
{
	Game();
	UserAnalysis();
	system("pause");
}

int RandNumb()
{
	srand((unsigned)time(NULL));
	int GameNumb = rand() % 100 + 1;
	return GameNumb;
}

int GameOS()
{
	int RandN = RandNumb();
	int UserGuess;
	int Times = 0;
	int FaultTimes = 0;
	int Min = 0, Max = 100;
	cout << "游戏【 从1~100猜出一个随机数 】" << endl;
	cout << endl;
	do
	{
		cout << "请输入你猜的数：";
		cin >> UserGuess;
		if (UserGuess <= Min || UserGuess > Max)
		{
			cout << "现在随机数范围被锁定在了 " << Min << " ~ " << Max << " 之间 " << endl;
			cout << endl;
			FaultTimes++;
			continue;
		}

		if (UserGuess > RandN)
		{
			cout << "比预设数字大！" << endl;
			cout << endl;
			if (UserGuess < Max)
				Max = UserGuess;
			Times++;
		}
		if (UserGuess < RandN)
		{
			cout << "比预设数字小！" << endl;
			cout << endl;
			if (UserGuess > Min)
				Min = UserGuess;
			Times++;
		}
		if (UserGuess == RandN)
		{
			Times++;
			break;
		}
	} while (true);

	LoadUserData();
	g_UserData.SumSteps += Times;
	g_UserData.SumFaultTimes += FaultTimes;
	g_UserData.SumGameTimes++;
	SaveUserData();

	return Times;
}

void Game()
{
	char Sel;
	do
	{
		system("cls");
		cout << "开始游戏吧！" << endl;
		int Times = GameOS();
		cout << "游戏结束 ！ 您共猜了 " << Times << " 次就猜中了正确答案！" << endl;
		cout << "还要再来一局吗？【按Y再来一局】" << endl;
		Sel = getch();
	} while (Sel == 'y' || Sel == 'Y');
}

void UserAnalysis()
{
	system("cls");
	LoadUserData();
	cout << "*************************" << endl;
	cout << "天 津 科 技  数据分析中心" << endl;
	cout << "*************************" << endl;
	cout << endl;
	cout << "您 共 进行过 " << g_UserData.SumGameTimes << " 次游戏" << endl;
	cout << endl;
	cout << "累计猜数次数 " << g_UserData.SumSteps << " 次" << endl;
	cout << endl;
	cout << "累计失误次数 " << g_UserData.SumFaultTimes << " 次" << endl;
	cout << endl;
	cout << "平均 " << (float)g_UserData.SumSteps / (float)g_UserData.SumGameTimes << " 次即可猜出随机数" << endl;
	cout << endl;
	cout << "平均失误次数 " << (float)g_UserData.SumFaultTimes / (float)g_UserData.SumGameTimes << " 次" << endl;
	cout << endl;
}

void SaveUserData()
{
	FILE *fp = fopen("DATA.lqm", "w");

	fwrite(&g_UserData, 1, sizeof(UserData), fp);

	fclose(fp);
}

void LoadUserData()
{
	FILE *fp = fopen("DATA.lqm", "a+");

	fread(&g_UserData, 1, sizeof(UserData), fp);

	fclose(fp);
}