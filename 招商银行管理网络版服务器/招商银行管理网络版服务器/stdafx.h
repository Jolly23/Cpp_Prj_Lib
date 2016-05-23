// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
/**************************************************/
/*******************ͷ�ļ�������*********************/

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <process.h>
#include <iostream>
#include <vector>


#include "SockJolly.h"
#include "BankBigData.h"
#include "Servers.h"
#include "Client.h"
#include "Admin.h"
/*********************ָ�����*********************/
enum Prior { P0 = 935007, P1 };
enum CardSecurity { Normal = 486, Freeze, Loss };
enum VIP { VIP0 = 4894613, VIP1, VIP2, VIP3, VIP4, VIP5 };
enum Sex { Male = 10086, Female };

enum ATM_REQ {
	ATMREQ_isCardNumbExist = 95531,
	ATMREQ_ClientLogin,
	ATMREQ_ApplyNewCardNumb,
	ATMREQ_CreateNewAccount,
	ATMREQ_AppleCardSecuritySituation,
	ATMREQ_ApplyDepositMoney,
	ATMREQ_CheckSumMoney,
	ATMREQ_GetVipLevel,
	ATMREQ_DrawMoneyAndPoundage,
	ATMREQ_TransferMoney,
	ATMREQ_ApplyCardBasicData,
	ATMREQ_GetCardPassword,
	ATMREQ_ApplyFreezeCard,
	ATMREQ_ApplyChangePassword
};

enum Man_REQ {
	MANREQ_isAdminLoginNameExist = 85531,
	MANREQ_AdminLogin,
	MANREQ_DownloadAllClientData,
	MANREQ_DownloadOneClientData,
	MANREQ_ModifyClientName,
	MANREQ_ModifyClientSex,
	MANREQ_ModifyClientSecurity
};

enum BigData_REQ {
	BDataREQ_GetBaseData = 75091,
};
/**************************************************/


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
