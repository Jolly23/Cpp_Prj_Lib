// ATM.h: interface for the CATM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATM_H__2A588536_165E_41EA_860F_94FC1BC89D1A__INCLUDED_)
#define AFX_ATM_H__2A588536_165E_41EA_860F_94FC1BC89D1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Client.h"
class CATM
{
	CClient m_Client;
	bool Deposit(int p_Client);
	int ATMMenu(int p_Client);
	char* Time();
public:
	bool ChangePassConfirm(ClientData *Client);
	bool ChangePass(int p_Client);
	bool TransferMoneyConfirm2(int p_Client, ClientData *ToCard, double dTranM, double dSum);
	bool TransferMoneyConfirm(int p_Client, ClientData *ToCard, double dTranM);
	void TransferMoney(int p_Client);
	void CheckMoney(int p_Client);
	bool DrawMontyConfirm2(int p_Client, double dDrawMoney);
	bool DrawMontyConfirm(int p_Client, int nDrawMoney);
	bool DrawMoney(int p_Client);
	CATM();
	virtual ~CATM();
};

#endif // !defined(AFX_ATM_H__2A588536_165E_41EA_860F_94FC1BC89D1A__INCLUDED_)
