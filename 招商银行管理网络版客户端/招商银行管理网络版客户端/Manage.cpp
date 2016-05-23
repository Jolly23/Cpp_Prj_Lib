#include "stdafx.h"
using namespace std;
int SortByCardNumb(const void *e1, const void *e2);
int SortByClientName(const void *e1, const void *e2);
int SortBySumMoney(const void *e1, const void *e2);
int SortByEarnForBank(const void *e1, const void *e2);


CManage::CManage()
{
	this->Login();
}


CManage::~CManage()
{
}


int CManage::Login()
{
	system("cls");

	AdminLoginData LoginD;
	cout << "请输入登录用户名：";
	cin >> LoginD.sLoginName;

	if (!Cmd_isAdminLoginNameExist(LoginD.sLoginName))
	{
		cout << "您输入的账户不存在！" << endl;
		system("pause");
		return -1;
	}

	cout << endl << "请输入密码：";
	int i = 0;
	char ch;
	while ((ch = getch()) != '\r')
	{
		if (ch == '\b')
		{
			--i;
			cout << '\b';
			cout << " ";
			cout << '\b';
			continue;
		}
		putchar('*');
		LoginD.sPass[i++] = ch;
	}
	LoginD.sPass[i] = '\0';

	char   sAdminName[20];	//员工姓名
	int    nSex;			//性别
	UINT   unPrior;			//权限

	if(Cmd_AdminLoginResult(LoginD,sAdminName, nSex, unPrior))
	{
		while (ManageMenu(LoginD.sLoginName, sAdminName, nSex, unPrior))
			;
		return 1;
	}
	else {
		cout << endl << "登陆失败！" << endl;
		system("pause");
		return -1;
	}
}

BOOL CManage::Cmd_isAdminLoginNameExist(char LoginName[])
{
	int nCmd = MANREQ_isAdminLoginNameExist, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(LoginName, 32);
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}

BOOL CManage::Cmd_AdminLoginResult(AdminLoginData LoginD, char rsAdminName[], int &rnSex, UINT &runPrior)
{
	int nCmd = MANREQ_AdminLogin, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&LoginD, sizeof(LoginD));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes) {
		//如果登陆成功，从服务器接收 管理员姓名、客户性别、权限
		sock.Receive(rsAdminName, 20);
		sock.Receive(&rnSex, sizeof(rnSex));
		sock.Receive(&runPrior, sizeof(runPrior));
		return TRUE;
	}
	else
		return FALSE;
}

int CManage::ManageMenu(char sLoginName[], char sAdminName[], int nSex, UINT unPrior)
{
	system("cls");
	cout << "******************************************" << endl;
	cout << "********尊敬的 " << sAdminName
		<< (nSex == Male ? " 先生," : " 女士,")
		<< Time() << "*******" << endl;
	cout << "******************************************" << endl;
	cout << "1.浏览全部客户账户" << endl;
	cout << "2.修改客户账户" << endl;
	cout << "3.冻结、解冻银行卡" << endl;
	cout << "4.挂失、解挂银行卡" << endl;
	cout << "5.浏览银行大数据库(每10分钟更新一次)" << endl;
	if (P0 == unPrior)
		cout << "6.管理银行管理员账户" << endl;
	cout << "0.退出" << endl;
	cout << "******************************************" << endl;

	char cSel = getch();
	if ('6' == cSel  &&  P1 == unPrior)
		return 1;
	switch (cSel)
	{
	case '0':
		return 0;
		break;
	case '1':
		DownloadAllClientDataAndBrowse();
		break;
	case '2':
		ModifyClientData();
		break;
	case '3':
//		FreezeOrOffCard();
		break;
	case '4':
//		LoseOrOffCard();
		break;
	case '5':
		BankDataBase();
		break;
	case '6':
//		SetAdminAccount();
		break;
	}
	return 1;
}

void CManage::DownloadAllClientDataAndBrowse()
{
	int nCmd = MANREQ_DownloadAllClientData, nCountC = 0, i = 0;
	ClientData LS_CLientD;
	vector<ClientData> LS_ClientVector;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Receive(&nCountC, sizeof(nCountC));
	while (i < nCountC)
	{
		sock.Receive(&LS_CLientD, sizeof(ClientData));
		LS_ClientVector.push_back(LS_CLientD);
		++i;
	}
	sock.Close();

	while (BrowseMenu(LS_ClientVector, nCountC))
		;
	LS_ClientVector.clear();
}

int CManage::BrowseMenu(vector<ClientData> &LS_ClientVector, int nCountC)
{
	system("cls");
	cout << "******************" << endl;
	cout << "1.按卡号排序浏览全部" << endl;
	cout << "2.按姓名排序浏览全部" << endl;
	cout << "3.按余额排序浏览全部" << endl;
	cout << "4.客户为银行盈利排序" << endl;
	cout << "5.按VIP等级分类浏览" << endl;
	cout << "6.输入卡号浏览单客户" << endl;
	
	cout << "0.退出" << endl;
	cout << "******************" << endl;
	char cSel = getch();
	switch (cSel)
	{
	case '0':
		return 0;
	case '1':
		qsort(&LS_ClientVector[0], nCountC, sizeof(ClientData), SortByCardNumb);
		PrintClientData(LS_ClientVector, nCountC);
		break;
	case '2':
		qsort(&LS_ClientVector[0], nCountC, sizeof(ClientData), SortByClientName);
		PrintClientData(LS_ClientVector, nCountC);
		break;
	case '3':
		qsort(&LS_ClientVector[0], nCountC, sizeof(ClientData), SortBySumMoney);
		PrintClientData(LS_ClientVector, nCountC);
		break;
	case '4':
		qsort(&LS_ClientVector[0], nCountC, sizeof(ClientData), SortByEarnForBank);
		PrintClientData(LS_ClientVector, nCountC);
		break;
	case '5':
		PrintInputVipLevelClient(LS_ClientVector, nCountC);
		break;
	case '6':
		PrintOneClientData(LS_ClientVector, nCountC);
		break;
	}
	return 1;
}

void CManage::PrintOneClientData(vector<ClientData> &LS_ClientVector, int nCountC)
{
	int nInputCardNumb, i = 0;
	cout << "请输入要浏览的客户的卡号（____）";
	cout << "\b\b\b\b\b\b";
	cin >> nInputCardNumb;
	while (i < nCountC)
	{
		if (nInputCardNumb == LS_ClientVector[i].nCardNumb) {
			PrintClientData(LS_ClientVector[i]);
			goto EXIT;
		}
		++i;
	}
	cout << "未找到卡号为 " << nInputCardNumb << " 的客户数据" << endl;

EXIT:
	system("pause");
}

void CManage::PrintInputVipLevelClient(std::vector<ClientData> &LS_ClientVector, int nCountC)
{
	int InputVipLevel, InputTimes = 0, nCountOfInputLevelClient = 0, i = 0;
	cout << "请输入要浏览的客户的等级：VIP（_）";
	cout << "\b\b\b";
	do {
		if (InputTimes)
			cout << "输入有误，请输入一个0~5之间的数字" << endl;
		cin >> InputVipLevel;
		++InputTimes;
	} while (InputVipLevel < 0 && InputVipLevel > 5);
	
	
	cout << "卡号\t姓名\t性别\t余 额 \t\t安全状态\t等级\t银行盈利" << endl;
	while (i < nCountC)
	{
		if (LS_ClientVector[i].nVIP - 4894613 == InputVipLevel)
		{
			PrintClientData(LS_ClientVector[i]);
			++nCountOfInputLevelClient;
		}
		++i;
	}
	cout << "招商银行目前共有 " << nCountOfInputLevelClient 
		<< " 位 VIP" << InputVipLevel << " 客户" << endl;
	system("pause");
}

void CManage::PrintClientData(vector<ClientData> &LS_ClientVector, int nCountC)
{
	int i = 0;
	cout << "招商银行目前共有 " << nCountC << " 位客户" << endl;
	cout << "卡号\t姓名\t性别\t余 额\t\t安全状态\t等级\t银行盈利" << endl;
	while (i < nCountC)
	{
		PrintClientData(LS_ClientVector[i]);
		++i;
	}
	system("pause");
}

void CManage::ModifyClientData()
{
	system("cls");
	ClientData ModifyData;
	int nCardNumb;
	cout << "请输入需要查询修改的卡号：";
	cin >> nCardNumb;
	if (!Cmd_DownloadOneClientData(nCardNumb, ModifyData)) {
		cout << "未找到卡号为 " << nCardNumb << " 的客户数据" << endl;
		system("pause");
		return;
	}

	cout << "卡号\t姓名\t性别\t余 额 \t\t安全状态\t等级\t银行盈利" << endl;
	PrintClientData(ModifyData);
	cout << "1.修改卡主姓名" << endl;
	cout << "2.修改卡主性别" << endl;
	cout << "3.修改安全状态" << endl;
	char sNewName[20];
	int nLSINT = 0;
	char cSel = getch();
	switch (cSel)
	{
	case '1':
		cout << "请输入修改后的持卡人姓名：";
		cin >> sNewName;
		cout << "确定将卡号为 " << nCardNumb << " 的持卡人姓名由原来的 "
			<< ModifyData.sName << " 修改为 " << sNewName << " 吗？【Y/N】" << endl;
		cSel = getch();
		if ('y' == cSel || 'Y' == cSel) {
			if (Cmd_ModifyClientName(nCardNumb, sNewName)) {
				cout << "修改成功！" << endl;
				system("pause");
				return;
			}
		}
		else {
			cout << "修改取消！" << endl;
			system("pause");
			return;
		}
		break;

	case '2':
		ModifyData.nSex == Male ? nLSINT = Female : nLSINT = Male;
		cout << "持卡人注册性别为 " << (ModifyData.nSex == Male ? " 男" : " 女") 
			<< " 要变更为 " << (ModifyData.nSex == Male ? " 女" : " 男") 
			<< " 吗？【Y/N】" << endl;
		
		cSel = getch();
		if ('y' == cSel || 'Y' == cSel) {
			if (Cmd_ModifyClientSex(nCardNumb)) {
				cout << "修改成功！" << endl;
				system("pause");
				return;
			}
		}
		else {
			cout << "修改取消！" << endl;
			system("pause");
			return;
		}
		break;







	case '3':
		if (Normal == ModifyData.nCardSecurity)
			cout << "卡片当前状态为 正常" << endl;
		if (Freeze == ModifyData.nCardSecurity)
			cout << "卡片当前状态为 冻结" << endl;
		if (Loss == ModifyData.nCardSecurity)
			cout << "卡片当前状态为 挂失" << endl;
		do {
			do {
				cout << "请选择目标状态：" << endl
					<< "1.正常" << endl
					<< "2.冻结" << endl
					<< "3.挂失" << endl;
				cSel = getch();
			} while (cSel != '1'  &&  cSel != '2'  &&  cSel == '3');

			switch (cSel)
			{
			case '1':
				nLSINT = Normal;
				break;
			case '2':
				nLSINT = Freeze;
				break;
			case '3':
				nLSINT = Loss;
				break;
			}
			if (nLSINT == ModifyData.nCardSecurity)
				cout << "输入有误，卡片状态未改变" << endl;
		} while (nLSINT == ModifyData.nCardSecurity);

		if (Normal == ModifyData.nCardSecurity)
			cout << "要将卡片状态由 正常" ;
		if (Freeze == ModifyData.nCardSecurity)
			cout << "要将卡片状态由 冻结" ;
		if (Loss == ModifyData.nCardSecurity)
			cout << "要将卡片状态由 挂失" ;

		if (Normal == nLSINT)
			cout << " 改为 正常 吗？【Y/N】" << endl;
		if (Freeze == nLSINT)
			cout << " 改为 冻结 吗？【Y/N】" << endl;
		if (Loss == nLSINT)
			cout << " 改为 挂失 吗？【Y/N】" << endl;

		cSel = getch();
		if ('y' == cSel || 'Y' == cSel) {
			if (Cmd_ModifyClientSecurity(nCardNumb, nLSINT)) {
				cout << "修改成功！" << endl;
				system("pause");
				return;
			}
		}
		else {
			cout << "修改取消！" << endl;
			system("pause");
			return;
		}
		break;




	}
}

BOOL CManage::Cmd_ModifyClientName(int nCardNumb, char sNewName[])
{
	int nCmd = MANREQ_ModifyClientName, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Send(sNewName, 20);
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes) 
		return TRUE;
	else
		return FALSE;
}

BOOL CManage::Cmd_ModifyClientSex(int nCardNumb)
{
	int nCmd = MANREQ_ModifyClientSex, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}

BOOL CManage::Cmd_ModifyClientSecurity(int nCardNumb, int nSecurity)
{
	int nCmd = MANREQ_ModifyClientSecurity, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Send(&nSecurity, sizeof(nSecurity));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}


BOOL CManage::Cmd_DownloadOneClientData(int nCardNumb, ClientData &rData)
{
	int nCmd = MANREQ_DownloadOneClientData, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes) {
		sock.Receive(&rData, sizeof(ClientData));
		return TRUE;
	}
	else
		return FALSE;
}

void CManage::PrintClientData(ClientData &CData)
{
	char sLS_Buff[20];
	cout << CData.nCardNumb << "\t"
		<< CData.sName << "\t"
		<< (CData.nSex == Male ? " 男" : " 女") << "\t";
	sprintf(sLS_Buff, "%.0lf", CData.dMoney);
	if (strlen(sLS_Buff) >= 8)
		cout << sLS_Buff << "\t";
	else
		cout << sLS_Buff << "\t\t";
	if (Normal == CData.nCardSecurity)
		cout << " 正常\t\t";
	if (Freeze == CData.nCardSecurity)
		cout << "*冻结\t\t";
	if (Loss == CData.nCardSecurity)
		cout << "*挂失\t\t";
	cout << "VIP" << CData.nVIP - 4894613 << "\t";
	sprintf(sLS_Buff, "%.0lf", CData.dPayForBank);
	cout << sLS_Buff << endl;
}

void CManage::BankDataBase()
{
	double dBankIncome, dBankSumMoney;
	UINT nBankSumClient;
	char sBigDataUpdataTimeBuff[64];
	char strBuff[64];
	GetBankData(sBigDataUpdataTimeBuff,dBankIncome, dBankSumMoney, nBankSumClient);
	cout << endl << "当前数据更新时间：" << sBigDataUpdataTimeBuff << endl;
	sprintf(strBuff,"当前银行客户总数：%d 人", nBankSumClient);
	cout << strBuff << endl;
	sprintf(strBuff, "当前银行存款总数：%.2lf 元（RMB）", dBankSumMoney);
	cout << strBuff << endl;
	sprintf(strBuff, "当前银行总收入：%.2lf 元（RMB）", dBankIncome);
	cout << strBuff << endl;
	system("pause");
}

BOOL CManage::GetBankData(char sTimeBuff[], double &rdBankIncome,
							double &rdBankSumMoney, UINT &rnBankSumClient)
{
	int nCmd = BDataREQ_GetBaseData, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Receive(sTimeBuff, 64);
	sock.Receive(&rdBankIncome, sizeof(rdBankIncome));
	sock.Receive(&rdBankSumMoney, sizeof(rdBankSumMoney));
	sock.Receive(&rnBankSumClient, sizeof(rnBankSumClient));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}

char* CManage::Time()
{
	char tmpbuf[128];
	_strtime(tmpbuf);
	char *pChar = new char[10];
	int Hours = atoi(&tmpbuf[0]);
	if (Hours >= 19 || Hours < 5)
		strcpy(pChar, "晚上好");
	if (Hours >= 5 && Hours < 11)
		strcpy(pChar, "上午好");
	if (Hours >= 11 && Hours < 13)
		strcpy(pChar, "中午好");
	if (Hours >= 13 && Hours < 19)
		strcpy(pChar, "下午好");
	return pChar;
}


int SortByCardNumb(const void *e1, const void *e2)
{
	ClientData *pClient_1 = (ClientData*)e1;
	ClientData *pClient_2 = (ClientData*)e2;
	if (pClient_1->nCardNumb > pClient_2->nCardNumb)
		return 1;
	if (pClient_1->nCardNumb < pClient_2->nCardNumb)
		return -1;
	return 0;
}

int SortByClientName(const void *e1, const void *e2)
{
	ClientData *pClient_1 = (ClientData*)e1;
	ClientData *pClient_2 = (ClientData*)e2;
	return strcmp(pClient_1->sName, pClient_2->sName);
}

int SortBySumMoney(const void *e1, const void *e2)
{
	ClientData *pClient_1 = (ClientData*)e1;
	ClientData *pClient_2 = (ClientData*)e2;
	if (pClient_1->dMoney < pClient_2->dMoney)
		return 1;
	if (pClient_1->dMoney > pClient_2->dMoney)
		return -1;
	return 0;
}

int SortByEarnForBank(const void *e1, const void *e2)
{
	ClientData *pClient_1 = (ClientData*)e1;
	ClientData *pClient_2 = (ClientData*)e2;
	if (pClient_1->dPayForBank < pClient_2->dPayForBank)
		return 1;
	if (pClient_1->dPayForBank > pClient_2->dPayForBank)
		return -1;
	return 0;
}