// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
/////////////////////////////////////////////////////////////
#include "targetver.h"
#include "SockJolly.h"
#include "SockJollyEx.h"
#include "Hallo.h"
#include "Manage.h"
#include "ATM.h"

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <iomanip>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////
// TODO:  在此处引用程序需要的其他头文件
