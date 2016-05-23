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
		strcpy(pDefault->sName, "CMBC总行");
		pDefault->nSex = Male;
		strcpy(pDefault->sLoginName, "adminp0");
		strcpy(pDefault->sPass, "123456");
		pDefault->lfSalary = 0.0;
		pDefault->unPrior = P0;
		AdminList.push_back(*pDefault);
		delete []pDefault;

		AdminData *pDefault1 = new AdminData;
		strcpy(pDefault1->sName, "CMBC分行");
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
		cout << "保存文件时失败！" << endl;
		return;
	}
	int i = 0;
	int nCountAL = AdminList.size();
	while (i < nCountAL)//内存->文件:依据循环到链表结尾
	{
		fwrite(&AdminList[i], 1, sizeof(AdminData), fpAL);//p ->data类型是struct SStud类型
		++i;
	}
	fclose(fpAL);
}

void CAdmin::SaveClientData()
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

BOOL CAdmin::isAdminLoginNameExist(SockJolly *pSocka)
{
	int nCountAL = AdminList.size(), i = 0, nRes;
	char sLoginNameSendToHere[32] = { 0 };
	if ((pSocka->Receive(sLoginNameSendToHere, 32)) <= 0)
		return FALSE;
	cout << "客户端请求函数: isAdminLoginNameExist---登录名=" << sLoginNameSendToHere << endl;
	while (i < nCountAL)
	{
		if (STRICMP(AdminList[i].sLoginName, sLoginNameSendToHere) == 0)
		{
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			cout << "客户端请求函数: isAdminLoginNameExist---结果：找到此登录名" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: isAdminLoginNameExist---结果：未找到此登录名" << endl;
	return TRUE;
}

BOOL CAdmin::AdminLogin(SockJolly *pSocka)
{
	int nCountAL = AdminList.size(), i = 0, nRes;
	AdminLoginData LoginD;
	if ((pSocka->Receive(&LoginD, sizeof(LoginD))) <= 0)
		return FALSE;
	cout << "客户端请求函数: AdminLogin---账号=" << LoginD.sLoginName << " 密码=" << LoginD.sPass << endl;
	while (i < nCountAL)
	{
		if (STRICMP(AdminList[i].sLoginName, LoginD.sLoginName) == 0) {
			if (0 == strcmp(AdminList[i].sPass, LoginD.sPass)) {
				cout << "客户端请求函数: AdminLogin---结果：密码匹配，登陆成功！" << endl;
				//登陆成功，向客户端发送 管理员姓名、客户性别、权限
				nRes = 1;
				pSocka->Send(&nRes, sizeof(nRes));
				pSocka->Send(&AdminList[i].sName, sizeof(AdminList[i].sName));
				pSocka->Send(&AdminList[i].nSex, sizeof(AdminList[i].nSex));
				pSocka->Send(&AdminList[i].unPrior, sizeof(AdminList[i].unPrior));
				return TRUE;
			}
			else {
				cout << "客户端请求函数: AdminLogin---结果：密码不匹配，登陆失败！" << endl;
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
	cout << "客户端请求函数: DownloadAllClientData" << endl;
	pSocka->Send(&nCountCL, sizeof(nCountCL));
	cout << "客户端请求函数: DownloadAllClientData---通知客户端客户总数=" << nCountCL << endl;
	while (i < nCountCL)
	{
		pSocka->Send(&ClientList[i], sizeof(ClientData));
		++i;
	}
	cout << "客户端请求函数: DownloadAllClientData---全部客户发送完毕" << endl;
	return TRUE;
}

BOOL CAdmin::DownloadOneClientData(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: DownloadOneClientData---收到卡号=" << nCardNumb << endl;
	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			nRes = 1;
			pSocka->Send(&nRes, sizeof(nRes));
			pSocka->Send(&ClientList[i], sizeof(ClientData));
			cout << "客户端请求函数: DownloadOneClientData---已找到，客户数据发送完毕" << endl;
			return TRUE;
		}
		++i;
	}
	nRes = -1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: DownloadOneClientData---未找到此客户" << endl;
	return TRUE;
}

BOOL CAdmin::ModifyClientName(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb;
	char sNewName[20];
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ModifyClientName---收到需要修改的卡号=" << nCardNumb << endl;
	if ((pSocka->Receive(sNewName, 20)) <= 0)
		return FALSE;
	cout << "客户端请求函数: ModifyClientName---收到新客户姓名=" << sNewName << endl;


	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			strcpy(ClientList[i].sName, sNewName);
			cout << "客户端请求函数: ModifyClientName---客户姓名修改完成" << endl;
			this->SaveClientData();
			cout << "客户端请求函数: ModifyClientName---银行数据库保存成功" << endl;
			break;
		}		
		++i;
	}
	nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: ModifyClientName---通知客户端修改完毕" << endl;
	return TRUE;
}

BOOL CAdmin::ModifyClientSex(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ModifyClientSex---收到需要修改的卡号=" << nCardNumb << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			ClientList[i].nSex == Male ? ClientList[i].nSex = Female : ClientList[i].nSex = Male;
			cout << "客户端请求函数: ModifyClientSex---客户姓名修改完成" << endl;
			this->SaveClientData();
			cout << "客户端请求函数: ModifyClientSex---银行数据库保存成功" << endl;
			break;
		}
		++i;
	}
	nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: ModifyClientSex---通知客户端修改完毕" << endl;
	return TRUE;
}

BOOL CAdmin::ModifyClientSecurity(SockJolly *pSocka)
{
	int nCountCL = ClientList.size(), i = 0, nRes = 0, nCardNumb,nToSecurity;
	if ((pSocka->Receive(&nCardNumb, sizeof(nCardNumb))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ModifyClientSex---收到需要修改的卡号=" << nCardNumb << endl;
	if ((pSocka->Receive(&nToSecurity, sizeof(nToSecurity))) <= 0)
		return FALSE;
	cout << "客户端请求函数: ModifyClientSex---收到要变更安全状态=" << nToSecurity << endl;

	while (i < nCountCL)
	{
		if (ClientList[i].nCardNumb == nCardNumb) {
			ClientList[i].nCardSecurity = nToSecurity;
			cout << "客户端请求函数: ModifyClientSex---卡片安全状态修改完成" << endl;
			this->SaveClientData();
			cout << "客户端请求函数: ModifyClientSex---银行数据库保存成功" << endl;
			break;
		}
		++i;
	}
	nRes = 1;
	pSocka->Send(&nRes, sizeof(nRes));
	cout << "客户端请求函数: ModifyClientSex---通知客户端修改完毕" << endl;
	return TRUE;
}