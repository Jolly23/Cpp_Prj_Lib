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
	std::cout << "��������_Designed By ZhaoLei" << std::endl;
	std::cout << "1.ʹ���������к�̨����ϵͳ" << std::endl;
	std::cout << "2.ʹ���������� ATM ��ȡ��" << std::endl;
	std::cout << "3.����ע�᱾�л����˻�" << std::endl;
	std::cout << "0.------�˳�-------" << std::endl;

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
