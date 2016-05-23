#include "stdafx.h"
/*
#include <windows.h>	//_sleepͷ�ļ������ܷ���ͷ�ļ���
*/

using namespace std;
extern std::vector<ClientData> ClientList;
extern std::vector<AdminData> AdminList;

PROC_RET_TYPE theProc(void* p);
PROC_RET_TYPE theServersSelf(void* p);

const int ServerPort = 40151;

UINT Times   = 0;	//�����Ӵ���
UINT Threads = 0;	//��ǰ������

CServers::CServers()
{
	this->LoadBankData();
#ifdef _WIN32
	_beginthread(theServersSelf, 0, 0);
#else
	pthread_t id = 0;
	pthread_create(&id, NULL, theServersSelf, 0);
#endif	
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
		cout << "�˿ڴ���ʧ��!  Error=" << WSAGetLastError() << endl;
		return 0;
	}

	this->Listen();

	char szIP[20];
	UINT nPort;
	char tmpbuf[128] = { 0 };

	SockJolly *pSocka = new SockJolly;

	cout << "/**************    �����������ɹ�    **************/" << endl;
//	cout << "/**************    ʱ�䣺" << _strtime(tmpbuf) << "    **************/" <<  endl;
	cout << "/**************************************************/" << endl;

	while (this->Accept(*pSocka, szIP, &nPort))
	{
		cout << endl << "�ͻ��˽��룺" << szIP << " - " << nPort << endl;
//		cout << "ʱ�䣺" << _strtime(tmpbuf) << endl;
#ifdef _WIN32
		_beginthread(theProc, 0, pSocka);
#else
		pthread_t id = 0;
		pthread_create(&id, NULL, theProc, pSocka);
#endif		
		pSocka = new SockJolly;
	}
	delete pSocka;
}


PROC_RET_TYPE theProc(void* p)
{
	++Threads;			//�ͻ������ӣ����ӵ�ǰ������
	SockJolly *pSocka = (SockJolly*)p;
	while (CServers::BankServers.OnReceive(pSocka))
		;
	pSocka->Close();
	delete(pSocka);
	--Threads;			//�ͻ��˶Ͽ����ӣ����ٵ�ǰ������
}



BOOL CServers::OnReceive(SockJolly *pSocka)
{
	int nCmd = 0;
	if ((pSocka->Receive(&nCmd, sizeof(nCmd))) <= 0)
		return FALSE;

	++Times;		//�ͻ������ӣ����ӷ��������Ӵ���

	cout << "__�� " << Times << " ��__�յ��ͻ���ָ��=" << nCmd << endl;
	cout << "��ǰ������=" << Threads << endl;
	switch (nCmd)
	{
	case 1111:
		cout << "�ͻ���-���� �� ���Է�������������" << endl;
		break;

	case 1112:
		cout << "�ͻ���-�ر�" << endl;
		break;


	//ATM��������
	case ATMREQ_isCardNumbExist:
		cout << "�ͻ���������: isCardNumbExist" << endl;
		ClientServer.isCardNumbExist(pSocka);
		break;

	case ATMREQ_ClientLogin:
		cout << "�ͻ���������: ClientLogin" << endl;
		ClientServer.ClientLogin(pSocka);
		break;

	case ATMREQ_ApplyNewCardNumb:
		cout << "�ͻ���������: ApplyNewCardNumb" << endl;
		ClientServer.ApplyNewCardNumb(pSocka);
		break;

	case ATMREQ_CreateNewAccount:
		cout << "�ͻ���������: CreateNewAccount" << endl;
		ClientServer.CreateNewAccount(pSocka);
		break;
		
	case ATMREQ_AppleCardSecuritySituation:
		cout << "�ͻ���������: AppleCardSecuritySituation" << endl;
		ClientServer.AppleCardSecuritySituation(pSocka);
		break;

	case ATMREQ_ApplyDepositMoney:
		cout << "�ͻ���������: ApplyDepositMoney" << endl;
		ClientServer.ApplyDepositMoney(pSocka);
		break;

	case ATMREQ_CheckSumMoney:
		cout << "�ͻ���������: CheckSumMoney" << endl;
		ClientServer.CheckSumMoney(pSocka);
		break;

	case ATMREQ_GetVipLevel:
		cout << "�ͻ���������: GetVipLevel" << endl;
		ClientServer.GetVipLevel(pSocka);
		break;
		
	case ATMREQ_DrawMoneyAndPoundage:
		cout << "�ͻ���������: DrawMoneyAndPoundage" << endl;
		ClientServer.DrawMoneyAndPoundage(pSocka);
		break;

	case ATMREQ_TransferMoney:
		cout << "�ͻ���������: TransferMoney" << endl;
		ClientServer.TransferMoney(pSocka);
		break;

	case ATMREQ_ApplyCardBasicData:
		cout << "�ͻ���������: ApplyCardBasicData" << endl;
		ClientServer.ApplyCardBasicData(pSocka);
		break;	

	case ATMREQ_GetCardPassword:
		cout << "�ͻ���������: ApplyGetCardPassword" << endl;
		ClientServer.ApplyGetCardPassword(pSocka);
		break;

	case ATMREQ_ApplyFreezeCard:
		cout << "�ͻ���������: ApplyFreezeCard" << endl;
		ClientServer.ApplyFreezeCard(pSocka);
		break;
		
	case ATMREQ_ApplyChangePassword:
		cout << "�ͻ���������: ApplyChangePassword" << endl;
		ClientServer.ApplyChangePassword(pSocka);
		break;
		


	//����ϵͳ������
	case MANREQ_isAdminLoginNameExist:
		cout << "�ͻ���������: isAdminLoginNameExist" << endl;
		AdminServer.isAdminLoginNameExist(pSocka);
		break;


	case MANREQ_AdminLogin:
		cout << "�ͻ���������: AdminLogin" << endl;
		AdminServer.AdminLogin(pSocka);
		break;

	case MANREQ_DownloadAllClientData:
		cout << "�ͻ���������: DownloadAllClientData" << endl;
		AdminServer.DownloadAllClientData(pSocka);
		break;
		

	case MANREQ_DownloadOneClientData:
		cout << "�ͻ���������: DownloadOneClientData" << endl;
		AdminServer.DownloadOneClientData(pSocka);
		break;
		
	case MANREQ_ModifyClientName:
		cout << "�ͻ���������: ModifyClientName" << endl;
		AdminServer.ModifyClientName(pSocka);
		break;

	case MANREQ_ModifyClientSex:
		cout << "�ͻ���������: ModifyClientSex" << endl;
		AdminServer.ModifyClientSex(pSocka);
		break;

	case MANREQ_ModifyClientSecurity:
		cout << "�ͻ���������: ModifyClientSecurity" << endl;
		AdminServer.ModifyClientSecurity(pSocka);
		break;


	//������ϵͳ������
	case BDataREQ_GetBaseData:
		cout << "�ͻ���������: GetBaseData" << endl;
		BigData.GetBaseData(pSocka);
		break;
		
	}
	return TRUE;
}

PROC_RET_TYPE theServersSelf(void* p)
{
	while (true)
	{
		const UINT unMins = 10;
		CServers::BankServers.ServersBankDataUpdata(unMins);	//���������д����ݸ���
#ifdef _WIN32
		_sleep(60000 * unMins);				//���ø��¼������ǰΪÿ unMins ���Ӹ���һ��
#else
		sleep(60 * unMins);				//���ø��¼������ǰΪÿ unMins ���Ӹ���һ��
#endif	
	}
}

void CServers::ServersBankDataUpdata(const UINT unMins)
{
	BigData.DataUpdata(unMins);	//���д����ݸ������
}
