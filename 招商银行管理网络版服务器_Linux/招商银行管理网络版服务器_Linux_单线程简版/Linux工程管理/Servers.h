#pragma once
#include "SockJolly.h"
#include "Client.h"
#include "Admin.h"
#include "BankBigData.h"

struct BasicData
{
	char sName[20];		//�ֿ�������
	int  nSex;			//�ֿ����Ա�
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