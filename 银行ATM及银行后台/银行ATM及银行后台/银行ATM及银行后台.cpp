// 银行ATM及银行后台.cpp : 定义控制台应用程序的入口点。
//


/*
软件需求：
银行：
完整的用户登录、银行内部管理、数据监测分析功能；
1. 主界面选择进入 银行后台 or ATM机
2. 只有银行管理员账户确认后才能进行后台管理
3. 银行管理员账户有分级，分为：支行、分行、总行、央行；
{
(P1)分行：开户、修改密码 + 修改 账户姓名、身份证号、挂失账户
(P0)总行：*分行权限 + 直接修改账户余额 + 银行数据分析 + 冻结账户 + 查看&修改管理员账户信息
}
*每个管理员都可以修改自己的账户密码(不能修改管理员个人信息)

用户：
ATM机存取转账操作;
1. 凭用户名+密码 登录ATM机
2. 存款操作
3. 取款操作(普通卡不能透支，信用卡可透支)
4. 对账号进行直接转账(确认时只显示 姓+称呼)
5. 修改取款密码
*/


#include "stdafx.h"
#include "BankAdmin.h"
#include "ATM.h"
#include <conio.h>
#include <time.h>
#include <iostream>
using std::cout;
using std::endl;
void Menu();

void main()
{
	do {
		system("cls");
		Menu();
	} while (true);
}

void Menu()
{
	cout << "请选择登录类型" << endl
		<< "1.银行管理系统" << endl
		<< "2.ATM 自助系统" << endl
		<< "3.  自助开户" << endl
		<< "0.安全退出系统" << endl;
	char cSel = getch();

	if (cSel == '1') {
		CBankAdmin Admin;
		return;
	}

	if (cSel == '2') {
		CATM atm;
		return;
	}

	if (cSel == '3') {
		CClient New;
		New.Load();
		New.ApplyNewAccount();
		return;
	};

	if (cSel == '0')
		return;
}
