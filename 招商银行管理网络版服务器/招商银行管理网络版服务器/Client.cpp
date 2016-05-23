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
		cout << "�����ļ�ʱʧ�ܣ�" << endl;
		return;
	}
	int j = 0;
	int nCountCL = ClientList.size();
	while (j < nCountCL)//�ڴ�->�ļ�:����ѭ���������β
	{
		fwrite(&ClientList[j], 1, sizeof(ClientData), fpCL);//p ->data������struct SStud����
		++j;
	}
	fclose(fpCL);
}

BOOL CClient::isCardNumbExist(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: isCardNumbExist---����=" << nCardNumb << endl;
	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb)
		{
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "�ͻ���������: isCardNumbExist---������ҵ��˿�" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: isCardNumbExist---�����δ�ҵ��˿�" << endl;
	return TRUE;
}

BOOL CClient::ClientLogin(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes;
	ClientLoginData LoginD;
	if ((pSocka->Receive(&LoginD, sizeof(LoginD))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ClientLogin---����=" << LoginD.nCardNumb << " ����=" << LoginD.sPassword << endl;
	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == LoginD.nCardNumb) {
			if (0 == strcmp(ClientList[i].sPass, LoginD.sPassword)) {
				cout << "�ͻ���������: ClientLogin---���������ƥ�䣬��½�ɹ���" << endl;
				//��½�ɹ�����ͻ��˷��� �ͻ��������ͻ��Ա�
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				pSocka->Send(&ClientList[i].sName, sizeof(ClientList[i].sName));
				pSocka->Send(&ClientList[i].nSex, sizeof(ClientList[i].nSex));
				return TRUE;
			}
			else {
				cout << "�ͻ���������: ClientLogin---��������벻ƥ�䣬��½ʧ�ܣ�" << endl;
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
	cout << "�ͻ���������: ApplyNewCardNumb---�¿����ţ�" << NewCardNumb << endl;
	return TRUE;
}


BOOL CClient::CreateNewAccount(SockJolly *pSocka)
{
	ClientData NewAccount;
	if ((pSocka->Receive(&NewAccount, sizeof(NewAccount))) <= 0)
		return FALSE;

	cout << "�ͻ���������: CreateNewAccount---�յ��ͻ��˷��͵����˻���Ϣ" << endl;
	ClientList.push_back(NewAccount);
	cout << "�ͻ���������: CreateNewAccount---�˻��ɹ������������ݿ�" << endl;
	this->SaveClientData();
	cout << "�ͻ���������: CreateNewAccount---�������ݿⱣ��ɹ�" << endl;

	int nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: CreateNewAccount---֪ͨ�ͻ��˴����˻��ɹ�" << endl;
	return TRUE;
}


BOOL CClient::AppleCardSecuritySituation(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nCardNumb;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: CreateNewAccount---�յ���Ҫ���Ŀ���=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			pSocka->Send(&ClientList[i].nCardSecurity, sizeof(ClientList[i].nCardSecurity));
			cout << "�ͻ���������: CreateNewAccount---���ؽ��=" << ClientList[i].nCardSecurity << endl;
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
	cout << "�ͻ���������: ApplyDepositMoney---�յ�����=" << nDepositCardNumb << endl;

	if ((pSocka->Receive(&nDepositMoney, sizeof(nDepositMoney))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ApplyDepositMoney---�յ�������=" << nDepositMoney << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nDepositCardNumb) {
			ClientList[i].dMoney += nDepositMoney;
			ClientList[i].nVIP = VipLevelCalc(nDepositCardNumb, ClientList[i].dMoney);	//VIP�ȼ������ж�
			cout << "�ͻ���������: ApplyDepositMoney---VIP�ȼ�����ƥ�����" << endl;
			this->SaveClientData();
			cout << "�ͻ���������: ApplyDepositMoney---�������ݿⱣ��ɹ�" << endl;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "�ͻ���������: ApplyDepositMoney---��֪�ͻ��˴������" << endl;
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
	cout << "�ͻ���������: CheckSumMoney---�յ���ѯ����=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			cout << "�ͻ���������: CheckSumMoney---�������=" << ClientList[i].dMoney << endl;
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
	cout << "�ͻ���������: GetVipLevel---�յ���ѯ����=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			cout << "�ͻ���������: GetVipLevel---����VIP�ȼ�=" << ClientList[i].nVIP << endl;
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
	cout << "�ͻ���������: ApplyCardBasicData---�յ���ѯ����=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			pSocka->Send(&ClientList[i].nSex, sizeof(ClientList[i].nSex));
			cout << "�ͻ���������: ApplyCardBasicData---�����Ա�" << ClientList[i].nSex << endl;
			pSocka->Send(&ClientList[i].sName, 20);
			cout << "�ͻ���������: ApplyCardBasicData---��������" << ClientList[i].sName << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: ApplyCardBasicData---δ�ҵ�����" << endl;
	return TRUE;
}

BOOL CClient::ApplyGetCardPassword(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ApplyGetCardPassword---�յ���ѯ����=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			pSocka->Send(&ClientList[i].sPass, 7);
			cout << "�ͻ���������: ApplyGetCardPassword---��������" << ClientList[i].sPass << endl;
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
	cout << "�ͻ���������: ApplyFreezeCard---�յ����Ῠ��=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			ClientList[i].nCardSecurity = Freeze;
			this->SaveClientData();
			cout << "�ͻ���������: ApplyFreezeCard---�������ݿⱣ��ɹ�" << endl;
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "�ͻ���������: ApplyFreezeCard---֪ͨ�ͻ��˶���ɹ�" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: ApplyFreezeCard---֪ͨ�ͻ��˶���ʧ��" << endl;
	return TRUE;
}

BOOL CClient::ApplyChangePassword(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes = 0;
	char NewPass[7] = { 0 };
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ApplyChangePassword---�յ������뿨��=" << nCardNumb << endl;
	if ((pSocka->Receive(NewPass, 7)) <= 0)
		return FALSE;
	cout << "�ͻ���������: ApplyChangePassword---�յ�������=" << NewPass << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			strcpy(ClientList[i].sPass, NewPass);
			this->SaveClientData();
			cout << "�ͻ���������: ApplyChangePassword---�������ݿⱣ��ɹ�" << endl;
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "�ͻ���������: ApplyChangePassword---֪ͨ�ͻ��˸�����ɹ�" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: ApplyChangePassword---֪ͨ�ͻ��˸�����ʧ��" << endl;
	return TRUE;
}

BOOL CClient::DrawMoneyAndPoundage(SockJolly *pSocka)
{
	int nCardNumb, nCountCL = ClientList.size(), i = 0, nRes;
	double dSum, dPoundage;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: DrawMoneyAndPoundage---�յ�ȡ���=" << nCardNumb << endl;
	if ((pSocka->Receive(&dSum, sizeof(dSum))) <= 0)
		return FALSE;
	cout << "�ͻ���������: DrawMoneyAndPoundage---�յ��ۿ��ܶ�=" << dSum << endl;
	if ((pSocka->Receive(&dPoundage, sizeof(dPoundage))) <= 0)
		return FALSE;
	cout << "�ͻ���������: DrawMoneyAndPoundage---�յ������ѽ��=" << dPoundage << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb)
		{
			if (ClientList[i].dMoney >= dSum)
			{
				ClientList[i].dMoney -= dSum;
				ClientList[i].dPayForBank += dPoundage;
				ClientList[i].nVIP = VipLevelCalc(nCardNumb, ClientList[i].dMoney);	//VIP�ȼ������ж�
				cout << "�ͻ���������: DrawMoneyAndPoundage---VIP�ȼ�����ƥ�����" << endl;
				this->SaveClientData();
				cout << "�ͻ���������: DrawMoneyAndPoundage---�������ݿⱣ��ɹ�" << endl;
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "�ͻ���������: DrawMoneyAndPoundage---֪ͨ�ͻ���ȡ��ɹ�" << endl;
				return TRUE;
			}
			else {
				nRes = -1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "�ͻ���������: DrawMoneyAndPoundage---֪ͨ�ͻ������㣬ȡ��ʧ��" << endl;
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
	cout << "�ͻ���������: TransferMoney---�յ�ת������=" << nCardNumb << endl;
	if ((pSocka->Receive(&nToCardNumb, sizeof(nToCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: TransferMoney---�յ�ת�뿨��=" << nToCardNumb << endl;
	if ((pSocka->Receive(&dTranM, sizeof(dTranM))) <= 0)
		return FALSE;
	cout << "�ͻ���������: TransferMoney---�յ�ת�˽��=" << dTranM << endl;
	if ((pSocka->Receive(&dPoundage, sizeof(dPoundage))) <= 0)
		return FALSE;
	cout << "�ͻ���������: TransferMoney---�յ������ѽ��=" << dPoundage << endl;

	double dSum = dTranM + dPoundage;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb)
		{
			if (ClientList[i].dMoney >= dSum)
			{
				//ת�����ۿ�
				ClientList[i].dMoney -= dSum;
				ClientList[i].dPayForBank += dPoundage;
				ClientList[i].nVIP = VipLevelCalc(nCardNumb, ClientList[i].dMoney);	//VIP�ȼ������ж�
				cout << "�ͻ���������: TransferMoney---ת����VIP�ȼ�����ƥ�����" << endl;
				//ת�뿨����
				while (j < nCountCL)
				{
					if (ClientList[j].nCardNumb == nToCardNumb)
					{
						ClientList[j].dMoney += dTranM;
						ClientList[j].nVIP = VipLevelCalc(nToCardNumb, ClientList[j].dMoney);
						cout << "�ͻ���������: TransferMoney---ת�뿨VIP�ȼ�����ƥ�����" << endl;
						break;
					}
					++j;
				}
				this->SaveClientData();
				cout << "�ͻ���������: TransferMoney---�������ݿⱣ��ɹ�" << endl;
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "�ͻ���������: TransferMoney---֪ͨ�ͻ���ת�˳ɹ�" << endl;
				return TRUE;
			}
			else {
				nRes = -1;
				pSocka->Send(&nRes, sizeof(nRes));
				cout << "�ͻ���������: TransferMoney---֪ͨ�ͻ������㣬ת��ʧ��" << endl;
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
