#include "stdafx.h"
using namespace std;
vector<AdminData> AdminList;
extern vector<ClientData> ClientList;

#ifndef _WIN32
#define strcasecmp stricmp
#endif // !_WIN32

CAdmin::CAdmin()
{
/*
	AdminData *pDefault = new AdminData;
		strcpy(pDefault->sName, "CMBC����");
		pDefault->nSex = Male;
		strcpy(pDefault->sLoginName, "adminp0");
		strcpy(pDefault->sPass, "123456");
		pDefault->lfSalary = 0.0;
		pDefault->unPrior = P0;
		AdminList.push_back(*pDefault);
		delete []pDefault;

		AdminData *pDefault1 = new AdminData;
		strcpy(pDefault1->sName, "CMBC����");
		pDefault1->nSex = Male;
		strcpy(pDefault1->sLoginName, "adminp1");
		strcpy(pDefault1->sPass, "123456");
		pDefault1->lfSalary = 0.0;
		pDefault1->unPrior = P1;
		AdminList.push_back(*pDefault1);
		delete []pDefault1;

		SaveAdminData();
*/
}


CAdmin::~CAdmin()
{
}

void CAdmin::LoadAdminData()
{
	AdminList.clear();
	FILE* fpAL = fopen("./AdminData.lv", "rb");
	if (!fpAL)
		return;
	AdminData Adata;
	while (fread(&Adata, 1, sizeof(AdminData), fpAL)>0)
		AdminList.push_back(Adata);
	fclose(fpAL);
}

void CAdmin::SaveAdminData()
{
	FILE* fpAL = fopen("./AdminData.lv", "wb");
	if (!fpAL)
	{
		cout << "�����ļ�ʱʧ�ܣ�" << endl;
		return;
	}
	int i = 0;
	int nCountAL = AdminList.size();
	while (i < nCountAL)//�ڴ�->�ļ�:����ѭ���������β
	{
		fwrite(&AdminList[i], 1, sizeof(AdminData), fpAL);//p ->data������struct SStud����
		++i;
	}
	fclose(fpAL);
}

void CAdmin::SaveClientData()
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

BOOL CAdmin::isAdminLoginNameExist(SockJolly *pSocka)
{
	int nCountAL = AdminList.size(), i = 0, nRes;
	char sLoginNameSendToHere[32] = { 0 };
	if ((pSocka->Receive(sLoginNameSendToHere, 32)) <= 0)
		return FALSE;
	cout << "�ͻ���������: isAdminLoginNameExist---��¼��=" << sLoginNameSendToHere << endl;
	while (i < nCountAL)
	{
		if (STRICMP(AdminList[i].sLoginName, sLoginNameSendToHere) == 0)
		{
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "�ͻ���������: isAdminLoginNameExist---������ҵ��˵�¼��" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: isAdminLoginNameExist---�����δ�ҵ��˵�¼��" << endl;
	return TRUE;
}

BOOL CAdmin::AdminLogin(SockJolly *pSocka)
{
	int nCountAL = AdminList.size(), i = 0, nRes;
	AdminLoginData LoginD;
	if ((pSocka->Receive(&LoginD, sizeof(LoginD))) <= 0)
		return FALSE;
	cout << "�ͻ���������: AdminLogin---�˺�=" << LoginD.sLoginName << " ����=" << LoginD.sPass << endl;
	while (i < nCountAL)
	{
		if (STRICMP(AdminList[i].sLoginName, LoginD.sLoginName) == 0) {
			if (0 == strcmp(AdminList[i].sPass, LoginD.sPass)) {
				cout << "�ͻ���������: AdminLogin---���������ƥ�䣬��½�ɹ���" << endl;
				//��½�ɹ�����ͻ��˷��� ����Ա�������ͻ��Ա�Ȩ��
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				pSocka->Send(&AdminList[i].sName, sizeof(AdminList[i].sName));
				pSocka->Send(&AdminList[i].nSex, sizeof(AdminList[i].nSex));
				pSocka->Send(&AdminList[i].unPrior, sizeof(AdminList[i].unPrior));
				return TRUE;
			}
			else {
				cout << "�ͻ���������: AdminLogin---��������벻ƥ�䣬��½ʧ�ܣ�" << endl;
				nRes = -1;
				pSocka->Send(&nRes, sizeof(nRes));
				return TRUE;
			}
		}
		++i;
	}
	return TRUE;
}

BOOL CAdmin::DownloadAllClientData(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes;
	cout << "�ͻ���������: DownloadAllClientData" << endl;
	pSocka->Send(&nCountCL, sizeof(nCountCL));
	cout << "�ͻ���������: DownloadAllClientData---֪ͨ�ͻ��˿ͻ�����=" << nCountCL << endl;
	while (i < nCountCL)
	{
		pSocka->Send(&ClientList[i], sizeof(ClientData));
		++i;
	}
	cout << "�ͻ���������: DownloadAllClientData---ȫ���ͻ��������" << endl;
	return TRUE;
}

BOOL CAdmin::DownloadOneClientData(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: DownloadOneClientData---�յ�����=" << nCardNumb << endl;
	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			pSocka->Send(&ClientList[i], sizeof(ClientData));
			cout << "�ͻ���������: DownloadOneClientData---���ҵ����ͻ����ݷ������" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: DownloadOneClientData---δ�ҵ��˿ͻ�" << endl;
	return TRUE;
}

BOOL CAdmin::ModifyClientName(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb;
	char sNewName[20];
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ModifyClientName---�յ���Ҫ�޸ĵĿ���=" << nCardNumb << endl;
	if ((pSocka->Receive(sNewName, 20)) <= 0)
		return FALSE;
	cout << "�ͻ���������: ModifyClientName---�յ��¿ͻ�����=" << sNewName << endl;


	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			strcpy(ClientList[i].sName, sNewName);
			cout << "�ͻ���������: ModifyClientName---�ͻ������޸����" << endl;
			this->SaveClientData();
			cout << "�ͻ���������: ModifyClientName---�������ݿⱣ��ɹ�" << endl;
			break;
		}		
		++i;
	}
	nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: ModifyClientName---֪ͨ�ͻ����޸����" << endl;
	return TRUE;
}

BOOL CAdmin::ModifyClientSex(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ModifyClientSex---�յ���Ҫ�޸ĵĿ���=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			ClientList[i].nSex == Male ? ClientList[i].nSex = Female : ClientList[i].nSex = Male;
			cout << "�ͻ���������: ModifyClientSex---�ͻ������޸����" << endl;
			this->SaveClientData();
			cout << "�ͻ���������: ModifyClientSex---�������ݿⱣ��ɹ�" << endl;
			break;
		}
		++i;
	}
	nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: ModifyClientSex---֪ͨ�ͻ����޸����" << endl;
	return TRUE;
}

BOOL CAdmin::ModifyClientSecurity(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb,nToSecurity;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ModifyClientSex---�յ���Ҫ�޸ĵĿ���=" << nCardNumb << endl;
	if ((pSocka->Receive(&nToSecurity, sizeof(nToSecurity))) <= 0)
		return FALSE;
	cout << "�ͻ���������: ModifyClientSex---�յ�Ҫ�����ȫ״̬=" << nToSecurity << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			ClientList[i].nCardSecurity = nToSecurity;
			cout << "�ͻ���������: ModifyClientSex---��Ƭ��ȫ״̬�޸����" << endl;
			this->SaveClientData();
			cout << "�ͻ���������: ModifyClientSex---�������ݿⱣ��ɹ�" << endl;
			break;
		}
		++i;
	}
	nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "�ͻ���������: ModifyClientSex---֪ͨ�ͻ����޸����" << endl;
	return TRUE;
}