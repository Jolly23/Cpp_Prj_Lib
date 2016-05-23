#pragma once
#include "SockJolly.h"
#include "Client.h"
#include "Admin.h"
#include "BankBigData.h"

struct BasicData
{
	char sName[20];		//持卡人姓名
	int  nSex;			//持卡人性别
};

class CServers :
	public SockJolly
{
public:
	CAdmin  AdminServer;
	CClient ClientServer;
	CBankBigData BigData;

	static CServers BankServers;
	CServers();
	~CServers();
	BOOL Start();
	BOOL OnReceive(SockJolly * pSocka);
	void LoadBankData();
	void SaveBankData();
	void ServersBankDataUpdata(const UINT unMins);
};