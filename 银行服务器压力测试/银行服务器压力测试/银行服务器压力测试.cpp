// 银行服务器压力测试.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
void theProc(void* p);
UINT nProc = 0;

int main()
{
	do {
		int nTimes = 0, i = 0;
		fflush(stdin);
		std::cout << "当前进程数：" << nProc << std::endl;
		std::cout << "请输入测试次数：";
		std::cin >> nTimes;
		if (!nTimes)
			return -1;
		while (i < nTimes)
		{
			_sleep(10);
			_beginthread(theProc, 0, 0);
			std::cout << "正在启动第 " << i + 1 << " 条线程" << std::endl;
			++i;
		}
		std::cout << "当前未运行完进程数：" << nProc << std::endl;
		system("pause");
	} while (true);
	return 0;
}

void theProc(void* p)
{
	++nProc;
	SockJollyEx *pSock = new SockJollyEx;
	int nCardNumb = 1002, i = 0;
	int LS = 9999;
	int nCmd = ATMREQ_isCardNumbExist, nRes;
	double dMoney = 1.0;
	double dMoney0 = 0.0;
	ClientLoginData LoginD = { 1002,"111111" };

	while (nCmd < ATMREQ_ApplyFreezeCard)
	{
		if (nCmd == ATMREQ_isCardNumbExist) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
		}
		if (nCmd == ATMREQ_ClientLogin) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&LoginD, sizeof(LoginD));
			pSock->Receive(&nRes, sizeof(nRes));
		}
		if (nCmd == ATMREQ_ApplyNewCardNumb) {
			pSock->Send(&nCmd, sizeof(nCmd));
		}

		if (nCmd == ATMREQ_AppleCardSecuritySituation) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
		}
		if (nCmd == ATMREQ_ApplyDepositMoney) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
			pSock->Send(&dMoney0, sizeof(dMoney0));
		}
		if (nCmd == ATMREQ_CheckSumMoney) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
		}

		if (nCmd == ATMREQ_GetVipLevel) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
		}
		if (nCmd == ATMREQ_DrawMoneyAndPoundage) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
			pSock->Send(&dMoney, sizeof(dMoney));
			pSock->Send(&dMoney, sizeof(dMoney));
		}
		if (nCmd == ATMREQ_TransferMoney) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
			pSock->Send(&dMoney, sizeof(dMoney));
			pSock->Send(&dMoney, sizeof(dMoney));
		}
		if (nCmd == ATMREQ_GetCardPassword) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&nCardNumb, sizeof(nCardNumb));
		}
		if (nCmd == ATMREQ_ApplyFreezeCard) {
			pSock->Send(&nCmd, sizeof(nCmd));
			pSock->Send(&LS, sizeof(LS));
		}
		_sleep(500);
		++nCmd;
	}
	nCmd = 1112;
	pSock->Send(&nCmd, sizeof(nCmd));
	delete pSock;
	--nProc;
}