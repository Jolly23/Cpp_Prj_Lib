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
	cout << "�������¼�û�����";
	cin >> LoginD.sLoginName;

	if (!Cmd_isAdminLoginNameExist(LoginD.sLoginName))
	{
		cout << "��������˻������ڣ�" << endl;
		system("pause");
		return -1;
	}

	cout << endl << "���������룺";
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

	char   sAdminName[20];	//Ա������
	int    nSex;			//�Ա�
	UINT   unPrior;			//Ȩ��

	if(Cmd_AdminLoginResult(LoginD,sAdminName, nSex, unPrior))
	{
		while (ManageMenu(LoginD.sLoginName, sAdminName, nSex, unPrior))
			;
		return 1;
	}
	else {
		cout << endl << "��½ʧ�ܣ�" << endl;
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
		//�����½�ɹ����ӷ��������� ����Ա�������ͻ��Ա�Ȩ��
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
	cout << "********�𾴵� " << sAdminName
		<< (nSex == Male ? " ����," : " Ůʿ,")
		<< Time() << "*******" << endl;
	cout << "******************************************" << endl;
	cout << "1.���ȫ���ͻ��˻�" << endl;
	cout << "2.�޸Ŀͻ��˻�" << endl;
	cout << "3.���ᡢ�ⶳ���п�" << endl;
	cout << "4.��ʧ��������п�" << endl;
	cout << "5.������д����ݿ�(ÿ10���Ӹ���һ��)" << endl;
	if (P0 == unPrior)
		cout << "6.�������й���Ա�˻�" << endl;
	cout << "0.�˳�" << endl;
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
	cout << "1.�������������ȫ��" << endl;
	cout << "2.�������������ȫ��" << endl;
	cout << "3.������������ȫ��" << endl;
	cout << "4.�ͻ�Ϊ����ӯ������" << endl;
	cout << "5.��VIP�ȼ��������" << endl;
	cout << "6.���뿨��������ͻ�" << endl;
	
	cout << "0.�˳�" << endl;
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
	cout << "������Ҫ����Ŀͻ��Ŀ��ţ�____��";
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
	cout << "δ�ҵ�����Ϊ " << nInputCardNumb << " �Ŀͻ�����" << endl;

EXIT:
	system("pause");
}

void CManage::PrintInputVipLevelClient(std::vector<ClientData> &LS_ClientVector, int nCountC)
{
	int InputVipLevel, InputTimes = 0, nCountOfInputLevelClient = 0, i = 0;
	cout << "������Ҫ����Ŀͻ��ĵȼ���VIP��_��";
	cout << "\b\b\b";
	do {
		if (InputTimes)
			cout << "��������������һ��0~5֮�������" << endl;
		cin >> InputVipLevel;
		++InputTimes;
	} while (InputVipLevel < 0 && InputVipLevel > 5);
	
	
	cout << "����\t����\t�Ա�\t�� �� \t\t��ȫ״̬\t�ȼ�\t����ӯ��" << endl;
	while (i < nCountC)
	{
		if (LS_ClientVector[i].nVIP - 4894613 == InputVipLevel)
		{
			PrintClientData(LS_ClientVector[i]);
			++nCountOfInputLevelClient;
		}
		++i;
	}
	cout << "��������Ŀǰ���� " << nCountOfInputLevelClient 
		<< " λ VIP" << InputVipLevel << " �ͻ�" << endl;
	system("pause");
}

void CManage::PrintClientData(vector<ClientData> &LS_ClientVector, int nCountC)
{
	int i = 0;
	cout << "��������Ŀǰ���� " << nCountC << " λ�ͻ�" << endl;
	cout << "����\t����\t�Ա�\t�� ��\t\t��ȫ״̬\t�ȼ�\t����ӯ��" << endl;
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
	cout << "��������Ҫ��ѯ�޸ĵĿ��ţ�";
	cin >> nCardNumb;
	if (!Cmd_DownloadOneClientData(nCardNumb, ModifyData)) {
		cout << "δ�ҵ�����Ϊ " << nCardNumb << " �Ŀͻ�����" << endl;
		system("pause");
		return;
	}

	cout << "����\t����\t�Ա�\t�� �� \t\t��ȫ״̬\t�ȼ�\t����ӯ��" << endl;
	PrintClientData(ModifyData);
	cout << "1.�޸Ŀ�������" << endl;
	cout << "2.�޸Ŀ����Ա�" << endl;
	cout << "3.�޸İ�ȫ״̬" << endl;
	char sNewName[20];
	int nLSINT = 0;
	char cSel = getch();
	switch (cSel)
	{
	case '1':
		cout << "�������޸ĺ�ĳֿ���������";
		cin >> sNewName;
		cout << "ȷ��������Ϊ " << nCardNumb << " �ĳֿ���������ԭ���� "
			<< ModifyData.sName << " �޸�Ϊ " << sNewName << " �𣿡�Y/N��" << endl;
		cSel = getch();
		if ('y' == cSel || 'Y' == cSel) {
			if (Cmd_ModifyClientName(nCardNumb, sNewName)) {
				cout << "�޸ĳɹ���" << endl;
				system("pause");
				return;
			}
		}
		else {
			cout << "�޸�ȡ����" << endl;
			system("pause");
			return;
		}
		break;

	case '2':
		ModifyData.nSex == Male ? nLSINT = Female : nLSINT = Male;
		cout << "�ֿ���ע���Ա�Ϊ " << (ModifyData.nSex == Male ? " ��" : " Ů") 
			<< " Ҫ���Ϊ " << (ModifyData.nSex == Male ? " Ů" : " ��") 
			<< " �𣿡�Y/N��" << endl;
		
		cSel = getch();
		if ('y' == cSel || 'Y' == cSel) {
			if (Cmd_ModifyClientSex(nCardNumb)) {
				cout << "�޸ĳɹ���" << endl;
				system("pause");
				return;
			}
		}
		else {
			cout << "�޸�ȡ����" << endl;
			system("pause");
			return;
		}
		break;







	case '3':
		if (Normal == ModifyData.nCardSecurity)
			cout << "��Ƭ��ǰ״̬Ϊ ����" << endl;
		if (Freeze == ModifyData.nCardSecurity)
			cout << "��Ƭ��ǰ״̬Ϊ ����" << endl;
		if (Loss == ModifyData.nCardSecurity)
			cout << "��Ƭ��ǰ״̬Ϊ ��ʧ" << endl;
		do {
			do {
				cout << "��ѡ��Ŀ��״̬��" << endl
					<< "1.����" << endl
					<< "2.����" << endl
					<< "3.��ʧ" << endl;
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
				cout << "�������󣬿�Ƭ״̬δ�ı�" << endl;
		} while (nLSINT == ModifyData.nCardSecurity);

		if (Normal == ModifyData.nCardSecurity)
			cout << "Ҫ����Ƭ״̬�� ����" ;
		if (Freeze == ModifyData.nCardSecurity)
			cout << "Ҫ����Ƭ״̬�� ����" ;
		if (Loss == ModifyData.nCardSecurity)
			cout << "Ҫ����Ƭ״̬�� ��ʧ" ;

		if (Normal == nLSINT)
			cout << " ��Ϊ ���� �𣿡�Y/N��" << endl;
		if (Freeze == nLSINT)
			cout << " ��Ϊ ���� �𣿡�Y/N��" << endl;
		if (Loss == nLSINT)
			cout << " ��Ϊ ��ʧ �𣿡�Y/N��" << endl;

		cSel = getch();
		if ('y' == cSel || 'Y' == cSel) {
			if (Cmd_ModifyClientSecurity(nCardNumb, nLSINT)) {
				cout << "�޸ĳɹ���" << endl;
				system("pause");
				return;
			}
		}
		else {
			cout << "�޸�ȡ����" << endl;
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
		<< (CData.nSex == Male ? " ��" : " Ů") << "\t";
	sprintf(sLS_Buff, "%.0lf", CData.dMoney);
	if (strlen(sLS_Buff) >= 8)
		cout << sLS_Buff << "\t";
	else
		cout << sLS_Buff << "\t\t";
	if (Normal == CData.nCardSecurity)
		cout << " ����\t\t";
	if (Freeze == CData.nCardSecurity)
		cout << "*����\t\t";
	if (Loss == CData.nCardSecurity)
		cout << "*��ʧ\t\t";
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
	cout << endl << "��ǰ���ݸ���ʱ�䣺" << sBigDataUpdataTimeBuff << endl;
	sprintf(strBuff,"��ǰ���пͻ�������%d ��", nBankSumClient);
	cout << strBuff << endl;
	sprintf(strBuff, "��ǰ���д��������%.2lf Ԫ��RMB��", dBankSumMoney);
	cout << strBuff << endl;
	sprintf(strBuff, "��ǰ���������룺%.2lf Ԫ��RMB��", dBankIncome);
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
		strcpy(pChar, "���Ϻ�");
	if (Hours >= 5 && Hours < 11)
		strcpy(pChar, "�����");
	if (Hours >= 11 && Hours < 13)
		strcpy(pChar, "�����");
	if (Hours >= 13 && Hours < 19)
		strcpy(pChar, "�����");
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