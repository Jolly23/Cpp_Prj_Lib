#pragma once
#include "SockJolly.h"


struct AdminData
{
	char   sName[20];		//Ա������
	int    nSex;			//�Ա�
	double lfSalary;		//����
	char   sLoginName[32];	//��¼�˺�
	char   sPass[32];		//��¼����
	UINT   unPrior;			//Ȩ��
};

struct AdminLoginData
{
	char   sLoginName[32];	//��¼�˺�
	char   sPass[32];		//��¼����
};

class CAdmin :
	public SockJolly
{
public:
	CAdmin();
	~CAdmin();
	void LoadAdminData();
	void SaveAdminData();
	BOOL isAdminLoginNameExist(SockJolly *pSocka);
	BOOL AdminLogin(SockJolly *pSocka);
	BOOL DownloadAllClientData(SockJolly *pSocka);
	BOOL DownloadOneClientData(SockJolly *pSocka);
	BOOL ModifyClientName(SockJolly *pSocka);
	BOOL ModifyClientSex(SockJolly *pSocka);
	BOOL ModifyClientSecurity(SockJolly *pSocka);
	void SaveClientData();

#ifdef _WIN32
	int STRICMP(char str1[], char str2[]) {
		return stricmp(str1, str2);
	}
#else
#include <string.h>
	int STRICMP(char str1[], char str2[]) {
		return strcasecmp(str1, str2);
	}
#endif // !_WIN32
};

