// BankAdmin.h: interface for the CBankAdmin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANKADMIN_H__F05AD7BE_D520_4AEC_B3B6_E14316B42113__INCLUDED_)
#define AFX_BANKADMIN_H__F05AD7BE_D520_4AEC_B3B6_E14316B42113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SAdmin
{
	char   sName[20];		//员工姓名
	int    nSex;			//性别
	double lfSalary;		//工资
	char   sLoginName[32];	//登录账号
	char   sPass[32];		//登录密码
	unsigned int unPrior;	//权限
};

#include "Client.h"
#include "BankData.h"

class CBankAdmin
{
public:
//	CClient m_client;
//	CBankData m_BData;
	void Menu(int DAdmin);
	void LoginRoot();
	void Save();
	void Load();
	int CheckLoginName(char sLName[]);
	int Login();
	CBankAdmin();
	~CBankAdmin();
};

#endif // !defined(AFX_BANKADMIN_H__F05AD7BE_D520_4AEC_B3B6_E14316B42113__INCLUDED_)
