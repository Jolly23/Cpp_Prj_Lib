#pragma once
#include <vector>
#include "ATM.h"

struct AdminLoginData
{
	char   sLoginName[32];	//µÇÂ¼ÕËºÅ
	char   sPass[32];		//µÇÂ¼ÃÜÂë
};

struct SAdmin
{
	char   sName[20];		//Ô±¹¤ÐÕÃû
	int    nSex;			//ÐÔ±ð
	double lfSalary;		//¹¤×Ê
	char   sLoginName[32];	//µÇÂ¼ÕËºÅ
	char   sPass[32];		//µÇÂ¼ÃÜÂë
	UINT   unPrior;			//È¨ÏÞ
};

class CManage
{
public:
	CManage();
	~CManage();

	int Login();
	BOOL Cmd_AdminLoginResult(AdminLoginData LoginD, char rsAdminName[], int &rnSex, UINT &runPrior);
	int ManageMenu(char sLoginName[], char sAdminName[], int nSex, UINT unPrior);
	BOOL Cmd_isAdminLoginNameExist(char LoginName[]);
	char* Time();
	void DownloadAllClientDataAndBrowse();
	void PrintClientData(std::vector<ClientData> &LS_ClientVector, int nCountC);
	int BrowseMenu(std::vector<ClientData> &LS_ClientVector, int nCountC);
	void PrintInputVipLevelClient(std::vector<ClientData> &LS_ClientVector, int nCountC);
	void ModifyClientData();
	void PrintOneClientData(std::vector<ClientData> &LS_ClientVector, int nCountC);


	void PrintClientData(ClientData &CData);
	BOOL Cmd_ModifyClientName(int nCardNumb, char sNewName[]);
	BOOL Cmd_DownloadOneClientData(int nCardNumb, ClientData &rData);
	BOOL Cmd_ModifyClientSex(int nCardNumb);
	BOOL Cmd_ModifyClientSecurity(int nCardNumb, int nSecurity);

	void BankDataBase();
	BOOL GetBankData(char sTimeBuff[], double &rdBankIncome, 
						double &rdBankSumMoney, UINT &rnBankSumClient);
};

