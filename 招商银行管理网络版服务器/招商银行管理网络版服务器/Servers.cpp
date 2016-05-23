#include "stdafx.h"
#include <windows.h>	//_sleep头文件，不能放入头文件集

using namespace std;
extern std::vector<ClientData> ClientList;
extern std::vector<AdminData> AdminList;

void theProc(void* p);
void theServersSelf(void* p);
const int ServerPort = 40151;

UINT Times   = 0;	//总连接次数
UINT Threads = 0;	//当前进程数

CServers::CServers()
{
	this->LoadBankData();
	_beginthread(theServersSelf, 0, 0);
	this->Start();
}

CServers::~CServers()
{
	this->Close();
	this->SaveBankData();
}


void CServers::LoadBankData()
{
	AdminServer.LoadAdminData();
	ClientServer.LoadClientData();
}

void CServers::SaveBankData()
{
	AdminServer.SaveAdminData();
	ClientServer.SaveClientData();
}

BOOL CServers::Start()
{
	int nRes = this->Create(ServerPort);

	if (0 == nRes) {
		cout << "端口创建失败!  Error=" << WSAGetLastError() << endl;
		return 0;
	}

	this->Listen();

	char szIP[20];
	UINT nPort;
	char tmpbuf[128] = { 0 };

	SockJolly *pSocka = new SockJolly;

	cout << "/**************    服务器启动成功    **************/" << endl;
	cout << "/**************    时间：" << _strtime(tmpbuf) << "    **************/" <<  endl;
	cout << "/**************************************************/" << endl;

	while (this->Accept(*pSocka, szIP, &nPort))
	{
		cout << endl << "客户端接入：" << szIP << " - " << nPort << endl;
		cout << "时间：" << _strtime(tmpbuf) << endl;
		_beginthread(theProc, 0, pSocka);
		pSocka = new SockJolly;
	}
	delete pSocka;
}

void theProc(void* p)
{
	++Threads;			//客户端连接，增加当前进程数
	SockJolly *pSocka = (SockJolly*)p;
	while (CServers::BankServers.OnReceive(pSocka))
		;
	pSocka->Close();
	delete(pSocka);
	--Threads;			//客户端断开连接，减少当前进程数
}

BOOL CServers::OnReceive(SockJolly *pSocka)
{
	int nCmd = 0;
	if ((pSocka->Receive(&nCmd, sizeof(nCmd))) <= 0)
		return FALSE;

	++Times;		//客户端连接，增加服务器连接次数

	cout << "__第 " << Times << " 次__收到客户端指令=" << nCmd << endl;
	cout << "当前进程数=" << Threads << endl;
	switch (nCmd)
	{
	case 1111:
		cout << "客户端-启动 并 测试服务器网络链接" << endl;
		break;

	case 1112:
		cout << "客户端-关闭" << endl;
		break;


	//ATM机请求函数
	case ATMREQ_isCardNumbExist:
		cout << "客户端请求函数: isCardNumbExist" << endl;
		ClientServer.isCardNumbExist(pSocka);
		break;

	case ATMREQ_ClientLogin:
		cout << "客户端请求函数: ClientLogin" << endl;
		ClientServer.ClientLogin(pSocka);
		break;

	case ATMREQ_ApplyNewCardNumb:
		cout << "客户端请求函数: ApplyNewCardNumb" << endl;
		ClientServer.ApplyNewCardNumb(pSocka);
		break;

	case ATMREQ_CreateNewAccount:
		cout << "客户端请求函数: CreateNewAccount" << endl;
		ClientServer.CreateNewAccount(pSocka);
		break;
		
	case ATMREQ_AppleCardSecuritySituation:
		cout << "客户端请求函数: AppleCardSecuritySituation" << endl;
		ClientServer.AppleCardSecuritySituation(pSocka);
		break;

	case ATMREQ_ApplyDepositMoney:
		cout << "客户端请求函数: ApplyDepositMoney" << endl;
		ClientServer.ApplyDepositMoney(pSocka);
		break;

	case ATMREQ_CheckSumMoney:
		cout << "客户端请求函数: CheckSumMoney" << endl;
		ClientServer.CheckSumMoney(pSocka);
		break;

	case ATMREQ_GetVipLevel:
		cout << "客户端请求函数: GetVipLevel" << endl;
		ClientServer.GetVipLevel(pSocka);
		break;
		
	case ATMREQ_DrawMoneyAndPoundage:
		cout << "客户端请求函数: DrawMoneyAndPoundage" << endl;
		ClientServer.DrawMoneyAndPoundage(pSocka);
		break;

	case ATMREQ_TransferMoney:
		cout << "客户端请求函数: TransferMoney" << endl;
		ClientServer.TransferMoney(pSocka);
		break;

	case ATMREQ_ApplyCardBasicData:
		cout << "客户端请求函数: ApplyCardBasicData" << endl;
		ClientServer.ApplyCardBasicData(pSocka);
		break;	

	case ATMREQ_GetCardPassword:
		cout << "客户端请求函数: ApplyGetCardPassword" << endl;
		ClientServer.ApplyGetCardPassword(pSocka);
		break;

	case ATMREQ_ApplyFreezeCard:
		cout << "客户端请求函数: ApplyFreezeCard" << endl;
		ClientServer.ApplyFreezeCard(pSocka);
		break;
		
	case ATMREQ_ApplyChangePassword:
		cout << "客户端请求函数: ApplyChangePassword" << endl;
		ClientServer.ApplyChangePassword(pSocka);
		break;
		


	//管理系统请求函数
	case MANREQ_isAdminLoginNameExist:
		cout << "客户端请求函数: isAdminLoginNameExist" << endl;
		AdminServer.isAdminLoginNameExist(pSocka);
		break;


	case MANREQ_AdminLogin:
		cout << "客户端请求函数: AdminLogin" << endl;
		AdminServer.AdminLogin(pSocka);
		break;

	case MANREQ_DownloadAllClientData:
		cout << "客户端请求函数: DownloadAllClientData" << endl;
		AdminServer.DownloadAllClientData(pSocka);
		break;
		

	case MANREQ_DownloadOneClientData:
		cout << "客户端请求函数: DownloadOneClientData" << endl;
		AdminServer.DownloadOneClientData(pSocka);
		break;
		
	case MANREQ_ModifyClientName:
		cout << "客户端请求函数: ModifyClientName" << endl;
		AdminServer.ModifyClientName(pSocka);
		break;

	case MANREQ_ModifyClientSex:
		cout << "客户端请求函数: ModifyClientSex" << endl;
		AdminServer.ModifyClientSex(pSocka);
		break;

	case MANREQ_ModifyClientSecurity:
		cout << "客户端请求函数: ModifyClientSecurity" << endl;
		AdminServer.ModifyClientSecurity(pSocka);
		break;


	//大数据系统请求函数
	case BDataREQ_GetBaseData:
		cout << "客户端请求函数: GetBaseData" << endl;
		BigData.GetBaseData(pSocka);
		break;
		
	}
	return TRUE;
}

void theServersSelf(void* p)
{
	while (true)
	{
		const UINT unMins = 1;
		CServers::BankServers.ServersBankDataUpdata(unMins);	//服务器银行大数据更新
		_sleep(60000 * unMins);				//设置更新间隔，当前为每 unMins 分钟更新一次
	}
}

void CServers::ServersBankDataUpdata(const UINT unMins)
{
	BigData.DataUpdata(unMins);	//银行大数据更新入口
}