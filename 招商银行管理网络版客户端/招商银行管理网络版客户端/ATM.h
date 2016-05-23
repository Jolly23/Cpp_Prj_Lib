#pragma once

struct ClientLoginData
{
	int  nCardNumb;
	char sPassword[7];
};

struct BasicData
{
	char sName[20];		//持卡人姓名
	int  nSex;			//持卡人性别
};

struct ClientData
{
	char sName[20];		//持卡人姓名
	int  nSex;			//持卡人性别
	int  nCardNumb;		//卡号(作为登录名)
	char sPass[7];		//六位密码
	double dMoney;		//余额
	int  nVIP;			//客户等级(VIP0 ~ VIP6)
	double dOverdraft;	//透支额度( = 0 则为储蓄卡用户，非 0 则为信用卡用户)
	double dOverdraftMoney; //已透支额度
	int  nCardSecurity;	//卡片安全状态(挂失、冻结)
	double dPayForBank;	//给银行带来的收入
};

class CATM 
{
public:
	CATM();
	~CATM();
	int    Login();
	int    ATMMenu(int ClientCardNumb, char sClientName[], int nSex);
	char*  Time();
	char*  VipDiscountText(int VipLevel);
	void   ApplyNewAccount();
	double GetVipDiscount(int nVipLevel);	
	BOOL Deposit(int DepositCardNumb, char sClientName[], int nSex);
	BOOL CheckMoney(int DepositCardNumb, char sClientName[], int nSex);
	BOOL DrawMoney(int DrawMoneyCardNumb, char sClientName[], int nSex);
	BOOL DrawMontyConfirm(int DrawMoneyCardNumb, int nDrawMoney);	
	BOOL TransferMoney(int nTransferMoneyCardNumb, char sClientName[], int nSex);
	BOOL TransferMoneyConfirm(int nTransferMoneyCardNumb, int nToCardNumb, double dTranM);	
	BOOL ChangePass(int nCardNumb);
	BOOL ChangePassConfirm(int nCardNumb);


	BOOL Cmd_ApplyChangePassword(int nCardNumb, char NewPass[]);
	BOOL Cmd_DrawMontyConfirm2(int DrawMoneyCardNumb, double dSum, double dPoundage);
	BOOL Cmd_TransferMoneyConfirm2(int nTransferMoneyCardNumb, int nToCardNumb, double dTranM, double dPoundage);
	BOOL Cmd_ApplyFreezeCard(int nCardNumb);
	BOOL Cmd_GetCardBasicData(int nCardNumb, BasicData &rBasicData);
	BOOL Cmd_ApplyDepositMoney(int nDepositCardNumb, int nDepositMoney);
	BOOL Cmd_SendNewApplyAccountInfo(ClientData &NewData);
	BOOL Cmd_isCardNumbExist(int nLNumb);
	BOOL Cmd_isLoginSucceed(ClientLoginData LoginD, char rClientName[], int &rnSex);
	int  Cmd_GetVipLevel(int nCardNumb);
	int  Cmd_ApplyNewCardNumb();
	int  Cmd_AppleCardSecuritySituation(int nCardNumb);
	double Cmd_CheckSumMoney(int nCardNumb);
	char*  Cmd_GetCardPassword(int nCardNumb);


};