#include "stdafx.h"

CHallo::CHallo()
{
}


CHallo::~CHallo()
{
}


int CHallo::HalloMenu()
{
	system("cls");
	std::cout << "招商银行_Designed By ZhaoLei" << std::endl;
	std::cout << "1.使用招商银行后台管理系统" << std::endl;
	std::cout << "2.使用招商银行 ATM 存取机" << std::endl;
	std::cout << "3.自助注册本行活期账户" << std::endl;
	std::cout << "0.------退出-------" << std::endl;

	char cSel = getch();
	if ('1' == cSel) {
		CManage ManageServer;
		return 1;
	}
	if ('2' == cSel) {
		CATM ATM_Server;
		ATM_Server.Login();
		return 1;
	}
	if ('3' == cSel) {
		CATM ATM_Server;
		ATM_Server.ApplyNewAccount();
		return 1;
	}
	if ('0' == cSel) {
		return 0;
	}
	return 1;
}
