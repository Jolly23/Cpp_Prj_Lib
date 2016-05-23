#pragma once
#include "SockJolly.h"

struct ClientData
{
	char sName[20];			//持卡人姓名
	int  nSex;				//持卡人性别
	int  nCardNumb;			//卡号(作为登录名)
	char sPass[7];			//六位密码
	double dMoney;			//余额
	int  nVIP;				//客户等级(VIP0 ~ VIP6)
	double dOverdraft;		//透支额度( = 0 则为储蓄卡用户，非 0 则为信用卡用户)
	double dOverdraftMoney; //已透支额度
	int  nCardSecurity;		//卡片安全状态(挂失、冻结)
	double dPayForBank;		//给银行带来的收入
};

struct ClientLoginData
{
	int  nCardNumb;
	char sPassword[7];
};

class CClient :
	public SockJolly
{
public:
	CClient();
	~CClient();
	void LoadClientData();
	void SaveClientData();

	BOOL ClientLogin(SockJolly *pSocka);
	BOOL isCardNumbExist(SockJolly *pSocka);
	BOOL ApplyNewCardNumb(SockJolly *pSocka);
	BOOL CreateNewAccount(SockJolly *pSocka);
	BOOL AppleCardSecuritySituation(SockJolly *pSocka);
	BOOL ApplyDepositMoney(SockJolly *pSocka);
	BOOL CheckSumMoney(SockJolly *pSocka);
	BOOL GetVipLevel(SockJolly *pSocka);
	BOOL DrawMoneyAndPoundage(SockJolly *pSocka);
	BOOL TransferMoney(SockJolly *pSocka);
	BOOL ApplyCardBasicData(SockJolly *pSocka);
	BOOL ApplyGetCardPassword(SockJolly *pSocka);
	BOOL ApplyFreezeCard(SockJolly *pSocka);
	BOOL ApplyChangePassword(SockJolly *pSocka);
	int VipLevelCalc(int nCardNumb, double dSumMoney);
};