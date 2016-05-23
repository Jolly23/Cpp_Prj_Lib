#pragma once
#if !defined(AFX_CLIENT_H__F2EED9D4_9C81_4AA5_B01C_D4716248FAAD__INCLUDED_)
#define AFX_CLIENT_H__F2EED9D4_9C81_4AA5_B01C_D4716248FAAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
	double nPayForBank;	//给银行带来的收入
};

class CClient
{
public:
	void ApplyNewAccount();
	int isVIP(int CNumb);
	void Load();
	void Save();
	int  Login();
	int  CheckCardNumb(int Numb);
	CClient();
	~CClient();
};

#endif // !defined(AFX_CLIENT_H__F2EED9D4_9C81_4AA5_B01C_D4716248FAAD__INCLUDED_)
