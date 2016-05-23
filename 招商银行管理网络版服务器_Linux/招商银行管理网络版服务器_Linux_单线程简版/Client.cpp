#include "stdafx.h"
using namespace std;
vector<ClientData> ClientList;

CClient::CClient()
{
}


CClient::~CClient()
{
}

void CClient::LoadClientData()
{
	ClientList.clear();
	FILE* fpCL = fopen("./ClientData.lv", "rb");
	if (!fpCL)
		return;
	ClientData Cdata;
	while (fread(&Cdata, 1, sizeof(ClientData), fpCL)>0)
		ClientList.push_back(Cdata);
	fclose(fpCL);
}

void CClient::SaveClientData()
{
	FILE* fpCL = fopen("./ClientData.lv", "wb");
	if (!fpCL)
	{
		cout << "保存文件时失败！" << endl;
		return;
	}
	int j = 0;
	int nCountCL = ClientList.size();
	while (j < nCountCL)//内存->文件:依据循环到链表结尾
	{
		fwrite(&ClientList[j], 1, sizeof(ClientData), fpCL);//p ->data类型是struct SStud类型
		++j;
	}
	fclose(fpCL);
}

BOOL CClient::isCardNumbExist(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: isCardNumbExist---卡号=" << nCardNumb << endl;
	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb)
		{
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "客户端请求函数: isCardNumbExist---结果：找到此卡" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: isCardNumbExist---结果：未找到此卡" << endl;
	return TRUE;
}

BOOL CClient::ClientLogin(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes;
	ClientLoginData LoginD;
	if ((pSocka->Receive(&LoginD, sizeof(LoginD))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ClientLogin---卡号=" << LoginD.nCardNumb << " 密码=" << LoginD.sPassword << endl;
	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == LoginD.nCardNumb) {
			if (0 == strcmp(ClientList[i].sPass, LoginD.sPassword)) {
				cout << "客户端请求函数: ClientLogin---结果：密码匹配，登陆成功！" << endl;
				//登陆成功，向客户端发送 客户姓名、客户性别
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				pSocka->Send(&ClientList[i].sName, sizeof(ClientList[i].sName));
				pSocka->Send(&ClientList[i].nSex, sizeof(ClientList[i].nSex));
				return TRUE;
			}
			else {
				cout << "客户端请求函数: ClientLogin---结果：密码不匹配，登陆失败！" << endl;
				nRes = -1;
				pSocka->Send(&nRes, sizeof(nRes));
				return TRUE;
			}
		}
		++i;
	}
	return TRUE;
}

BOOL CClient::ApplyNewCardNumb(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), NewCardNumb;
	NewCardNumb = ClientList[nCountCL - 1].nCardNumb + 1;
	pSocka->Send(&NewCardNumb, sizeof(NewCardNumb));
	cout << "客户端请求函数: ApplyNewCardNumb---新卡卡号：" << NewCardNumb << endl;
	return TRUE;
}


BOOL CClient::CreateNewAccount(SockJolly *pSocka)
{
	ClientData NewAccount;
	if ((pSocka->Receive(&NewAccount, sizeof(NewAccount))) <= 0)
		return FALSE;

	cout << "客户端请求函数: CreateNewAccount---收到客户端发送的新账户信息" << endl;
	ClientList.push_back(NewAccount);
	cout << "客户端请求函数: CreateNewAccount---账户成功加入银行数据库" << endl;
	this->SaveClientData();
	cout << "客户端请求函数: CreateNewAccount---银行数据库保存成功" << endl;

	int nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: CreateNewAccount---通知客户端创建账户成功" << endl;
	return TRUE;
}


BOOL CClient::AppleCardSecuritySituation(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nCardNumb;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: CreateNewAccount---收到需要检查的卡号=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			pSocka->Send(&ClientList[i].nCardSecurity, sizeof(ClientList[i].nCardSecurity));
			cout << "客户端请求函数: CreateNewAccount---返回结果=" << ClientList[i].nCardSecurity << endl;
			return TRUE;
		}
		++i;
	}
	return TRUE;
}


BOOL CClient::ApplyDepositMoney(SockJolly *pSocka)
{
	int nDepositCardNumb, nDepositMoney, nCountCL = ClientList.size(), i = 0, nRes = 1;
	if ((pSocka->Receive(&nDepositCardNumb, sizeof(nDepositCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ApplyDepositMoney---收到存款卡号=" << nDepositCardNumb << endl;

	if ((pSocka->Receive(&nDepositMoney, sizeof(nDepositMoney))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ApplyDepositMoney---收到存入金额=" << nDepositMoney << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nDepositCardNumb) {
			ClientList[i].dMoney += nDepositMoney;
			ClientList[i].nVIP = VipLevelCalc(nDepositCardNumb, ClientList[i].dMoney);	//VIP等级重新判断
			cout << "客户端请求函数: ApplyDepositMoney---VIP等级重新匹配完成" << endl;
			this->SaveClientData();
			cout << "客户端请求函数: ApplyDepositMoney---银行数据库保存成功" << endl;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "客户端请求函数: ApplyDepositMoney---告知客户端存入完成" << endl;
			return TRUE;
		}
		++i;
	}

	return TRUE;
}

BOOL CClient::CheckSumMoney(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0;
	double dSumMoney;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: CheckSumMoney---收到查询卡号=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			cout << "客户端请求函数: CheckSumMoney---返回余额=" << ClientList[i].dMoney << endl;
			pSocka->Send(&ClientList[i].dMoney, sizeof(ClientList[i].dMoney));
			return TRUE;
		}
		++i;
	}
	return TRUE;
}


BOOL CClient::GetVipLevel(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: GetVipLevel---收到查询卡号=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			cout << "客户端请求函数: GetVipLevel---返回VIP等级=" << ClientList[i].nVIP << endl;
			pSocka->Send(&ClientList[i].nVIP, sizeof(ClientList[i].nVIP));
			return TRUE;
		}
		++i;
	}
	return TRUE;
}

BOOL CClient::ApplyCardBasicData(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes = 0;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ApplyCardBasicData---收到查询卡号=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			pSocka->Send(&ClientList[i].nSex, sizeof(ClientList[i].nSex));
			cout << "客户端请求函数: ApplyCardBasicData---返回性别" << ClientList[i].nSex << endl;
			pSocka->Send(&ClientList[i].sName, 20);
			cout << "客户端请求函数: ApplyCardBasicData---返回姓名" << ClientList[i].sName << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: ApplyCardBasicData---未找到数据" << endl;
	return TRUE;
}

BOOL CClient::ApplyGetCardPassword(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ApplyGetCardPassword---收到查询卡号=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			pSocka->Send(&ClientList[i].sPass, 7);
			cout << "客户端请求函数: ApplyGetCardPassword---返回密码" << ClientList[i].sPass << endl;
			return TRUE;
		}
		++i;
	}
	return TRUE;
}

BOOL CClient::ApplyFreezeCard(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes = 0;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ApplyFreezeCard---收到冻结卡号=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			ClientList[i].nCardSecurity = Freeze;
			this->SaveClientData();
			cout << "客户端请求函数: ApplyFreezeCard---银行数据库保存成功" << endl;
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "客户端请求函数: ApplyFreezeCard---通知客户端冻结成功" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: ApplyFreezeCard---通知客户端冻结失败" << endl;
	return TRUE;
}

BOOL CClient::ApplyChangePassword(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes = 0;
	char NewPass[7] = { 0 };
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ApplyChangePassword---收到改密码卡号=" << nCardNumb << endl;
	if ((pSocka->Receive(NewPass, 7)) <= 0)
		return FALSE;
	cout << "客户端请求函数: ApplyChangePassword---收到新密码=" << NewPass << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			strcpy(ClientList[i].sPass, NewPass);
			this->SaveClientData();
			cout << "客户端请求函数: ApplyChangePassword---银行数据库保存成功" << endl;
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "客户端请求函数: ApplyChangePassword---通知客户端改密码成功" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: ApplyChangePassword---通知客户端改密码失败" << endl;
	return TRUE;
}

BOOL CClient::DrawMoneyAndPoundage(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes;
	double dSum, dPoundage;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: DrawMoneyAndPoundage---收到取款卡号=" << nCardNumb << endl;
	if ((pSocka->Receive(&dSum, sizeof(dSum))) <= 0)
		return FALSE;
	cout << "客户端请求函数: DrawMoneyAndPoundage---收到扣款总额=" << dSum << endl;
	if ((pSocka->Receive(&dPoundage, sizeof(dPoundage))) <= 0)
		return FALSE;
	cout << "客户端请求函数: DrawMoneyAndPoundage---收到手续费金额=" << dPoundage << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb)
		{
			if (ClientList[i].dMoney >= dSum)
			{
				ClientList[i].dMoney -= dSum;
				ClientList[i].dPayForBank += dPoundage;
				ClientList[i].nVIP = VipLevelCalc(nCardNumb, ClientList[i].dMoney);	//VIP等级重新判断
				cout << "客户端请求函数: DrawMoneyAndPoundage---VIP等级重新匹配完成" << endl;
				this->SaveClientData();
				cout << "客户端请求函数: DrawMoneyAndPoundage---银行数据库保存成功" << endl;
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "客户端请求函数: DrawMoneyAndPoundage---通知客户端取款成功" << endl;
				return TRUE;
			}
			else {
				nRes = -1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "客户端请求函数: DrawMoneyAndPoundage---通知客户端余额不足，取款失败" << endl;
				return TRUE;
			}
		}
		++i;
	}
	return TRUE;
}

BOOL CClient::TransferMoney(SockJolly *pSocka)
{
	int nCardNumb, nToCardNumb, nCountCL = ClientList.size(), i = 0, j = 0, nRes;
	double dTranM, dPoundage;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: TransferMoney---收到转出卡号=" << nCardNumb << endl;
	if ((pSocka->Receive(&nToCardNumb, sizeof(nToCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: TransferMoney---收到转入卡号=" << nToCardNumb << endl;
	if ((pSocka->Receive(&dTranM, sizeof(dTranM))) <= 0)
		return FALSE;
	cout << "客户端请求函数: TransferMoney---收到转账金额=" << dTranM << endl;
	if ((pSocka->Receive(&dPoundage, sizeof(dPoundage))) <= 0)
		return FALSE;
	cout << "客户端请求函数: TransferMoney---收到手续费金额=" << dPoundage << endl;

	double dSum = dTranM + dPoundage;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb)
		{
			if (ClientList[i].dMoney >= dSum)
			{
				//转出卡扣款
				ClientList[i].dMoney -= dSum;
				ClientList[i].dPayForBank += dPoundage;
				ClientList[i].nVIP = VipLevelCalc(nCardNumb, ClientList[i].dMoney);	//VIP等级重新判断
				cout << "客户端请求函数: TransferMoney---转出卡VIP等级重新匹配完成" << endl;
				//转入卡增款
				while (j < nCountCL)
				{
					if (ClientList[j].nCardNumb == nToCardNumb)
					{
						ClientList[j].dMoney += dTranM;
						ClientList[j].nVIP = VipLevelCalc(nToCardNumb, ClientList[j].dMoney);
						cout << "客户端请求函数: TransferMoney---转入卡VIP等级重新匹配完成" << endl;
						break;
					}
					++j;
				}
				this->SaveClientData();
				cout << "客户端请求函数: TransferMoney---银行数据库保存成功" << endl;
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "客户端请求函数: TransferMoney---通知客户端转账成功" << endl;
				return TRUE;
			}
			else {
				nRes = -1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "客户端请求函数: TransferMoney---通知客户端余额不足，转账失败" << endl;
				return TRUE;
			}
		}
		++i;
	}
	return TRUE;
}

int CClient::VipLevelCalc(int nCardNumb, double dSumMoney)
{
	if (dSumMoney == 0.0)
		return VIP0;
	if (dSumMoney <= 500.0)
		return VIP1;
	if (dSumMoney <= 10000.0)
		return VIP2;
	if (dSumMoney <= 1000000.0)
		return VIP3;
	if (dSumMoney <= 10000000.0)
		return VIP4;
	return VIP5;
}
