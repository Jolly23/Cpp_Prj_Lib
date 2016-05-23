#include "stdafx.h"
using namespace std;


CATM::CATM()
{
}


CATM::~CATM()
{
}


void CATM::ApplyNewAccount()
{
	system("cls");
	ClientData *pData = new ClientData;
	cout << "����������������";
	cin >> pData->sName;
	char cSel;
	do {
		cout << "��ѡ���Ա�" << endl
			<< "1. ��" << endl
			<< "2. Ů" << endl;
		cSel = getch();
	} while (cSel != '1'  &&  cSel != '2');
	if (cSel == '1')
		pData->nSex = Male;
	else
		pData->nSex = Female;

	/********************************/
	char SPass[7], CPass[7];
	int nTimes = 0;

	do {
		if (nTimes != 0)
			cout << endl << "������������벻һ�£�����������" << endl;
		/********************************/
		//��һ����������
		cout << endl << "���趨���룺";
		int i = 0;
		char ch;
		while ((ch = getch()) != '\r'  &&  i < 6)
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
			SPass[i++] = ch;
		}
		SPass[6] = '\0';
		/********************************/
		//�ڶ�����������
		cout << endl << "��������һ�Σ�";
		i = 0;
		while ((ch = getch()) != '\r'  &&  i < 6)
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
			CPass[i++] = ch;
		}
		CPass[6] = '\0';
		/********************************/
		//����ȷ��
		if (strcmp(SPass, CPass) == 0)
			strcpy(pData->sPass, SPass);
		++nTimes;
	} while (strcmp(SPass, CPass));

	//����������뿨�ţ�
	pData->nCardNumb = Cmd_ApplyNewCardNumb();
	//¼���������
	pData->dMoney = 0.0;
	pData->dOverdraft = 0.0;
	pData->dOverdraftMoney = 0.0;
	pData->nVIP = VIP0;
	pData->nCardSecurity = Normal;
	pData->dPayForBank = 0.0;

	//������������¿ͻ���Ϣ
	Cmd_SendNewApplyAccountInfo(*pData);

	cout << endl << endl
		<< "��ӭ " << pData->sName << (pData->nSex == Male ? " ����" : " Ůʿ") << "��Ϊ�������пͻ�!" << endl
		<< "�������п���¼�û���(����)Ϊ " << pData->nCardNumb << endl;
	delete pData;

	system("pause");
}

int CATM::Login()
{
	system("cls");
	ClientLoginData LoginD;

	cout << "�������¼���ţ�";
	cin >> LoginD.nCardNumb;

	if (!Cmd_isCardNumbExist(LoginD.nCardNumb))
	{
		cout << "������Ŀ��Ų����ڣ�" << endl;
		system("pause");
		return -1;
	}

	cout << endl << "���������룺";
	int i = 0;
	char ch;
	while ((ch = getch()) != '\r'  &&  i < 6)
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
		LoginD.sPassword[i++] = ch;
	}
	LoginD.sPassword[6] = '\0';

	char sClientName[20] = {0};
	int rnSex = 0;
	if (Cmd_isLoginSucceed(LoginD,sClientName,rnSex))
	{
		while( ATMMenu(LoginD.nCardNumb, sClientName, rnSex) )
			;
		return 1;
	}
	else
	{
		cout << endl << "��½ʧ�ܣ�" << endl;
		system("pause");
		return -1;
	}
	return 1;
}

int CATM::ATMMenu(int ClientCardNumb, char sClientName[], int nSex)
{
	system("cls");
	cout << "******************************************" << endl;
	cout << "*********�𾴵� " << sClientName
		<< (nSex == Male ? " ����," : " Ůʿ,")
		<< Time() << "********" << endl;
	cout << "******************************************" << endl;
	cout << "**  1.���                      ȡ��.2  **" << endl
		<< "**  3.ת��                  ��ѯ���.4  **" << endl
		<< "**  5.�޸�����            ���ÿ�����.6  **" << endl
		<< "**                0.�˿�                **" << endl;
	cout << "******************************************" << endl;

	char cSel = getch();
	switch (cSel)
	{
	case '1':
		this->Deposit(ClientCardNumb, sClientName, nSex);
		break;
	case '2':
		this->DrawMoney(ClientCardNumb, sClientName, nSex);
		break;
	case '3':
		this->TransferMoney(ClientCardNumb, sClientName, nSex);
		break;
	case '4':
		this->CheckMoney(ClientCardNumb, sClientName, nSex);
		break;
	case '5':
		this->ChangePass(ClientCardNumb);
		break;
	case '0':
		return 0;
	};
	return 1;
}

BOOL CATM::Deposit(int DepositCardNumb, char sClientName[], int nSex)
{
	//��ʧ������״̬�����п���ֹ��Ǯ
	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(DepositCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
	{
		cout << "�޷�����˿����˿�Ŀǰ�ѱ�" << (nCardSecuritySituation == Freeze ? "����" : "��ʧ") << endl;
		system("pause");
		return FALSE;
	}
	
	//��Ƭ����״̬��
	int nDepositMoney = 0;
	do {
		cout << "�����������(100�ı���)��";
		cin >> nDepositMoney;
	} while (nDepositMoney % 100 != 0  && nDepositMoney == 0);

	Cmd_ApplyDepositMoney(DepositCardNumb, nDepositMoney);

	cout<< "������ɣ�" << endl
		<< sClientName
		<< (nSex == Male ? " ����" : "Ůʿ")
		<< "��������Ϊ "
		<< DepositCardNumb
		<< "�����п����Ϊ "
		<< Cmd_CheckSumMoney(DepositCardNumb)
		<< endl;

	system("pause");
	return TRUE;
}

BOOL CATM::DrawMoney(int DrawMoneyCardNumb, char sClientName[], int nSex)
{
	//��ʧ������״̬�����п���ֹȡǮ
	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(DrawMoneyCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
	{
		cout << "�޷�����˿����˿�Ŀǰ�ѱ�" << (nCardSecuritySituation == Freeze ? "����" : "��ʧ") << endl;
		system("pause");
		return FALSE;
	}

	//��Ƭ����״̬��
	system("cls");
	int nDrawMoney;
	cout << "******************************************" << endl;
	cout << "**  1.--100Ԫ               1000Ԫ--.4  **" << endl
		<< "**  2.--300Ԫ               3000Ԫ--.5  **" << endl
		<< "**  3.--500Ԫ             ������--.6  **" << endl
		<< "**                0.ȡ��                **" << endl;
	cout << "******************************************" << endl;
	char cSel = getch();
	switch (cSel)
	{
	case '1':
		nDrawMoney = 100;
		break;
	case '2':
		nDrawMoney = 300;
		break;
	case '3':
		nDrawMoney = 500;
		break;
	case '4':
		nDrawMoney = 1000;
		break;
	case '5':
		nDrawMoney = 3000;
		break;
	case '6':
		do {
			cout << "������ȡ����(100�ı���)��" << endl;
			cin >> nDrawMoney;
		} while (nDrawMoney % 100 != 0);
		break;
	case '0':
		return false;
	};
	DrawMontyConfirm(DrawMoneyCardNumb, nDrawMoney);
	CheckMoney(DrawMoneyCardNumb, sClientName,nSex);

	return TRUE;
}

BOOL CATM::DrawMontyConfirm(int DrawMoneyCardNumb, int nDrawMoney)
{
	system("cls");

	int VipLevel = Cmd_GetVipLevel(DrawMoneyCardNumb);			//��ȡVIP�ȼ�
	double VipDiscount = GetVipDiscount(VipLevel);			//��ȡVIP�ۿ�
	
	cout << "******************************************************" << endl;
	cout << "�����Ѽ������" << endl
		<< "��������ȡ���� 1% ����Χ�� 2~50Ԫ֮��" << endl
		<< "������ȡ�������������ѵ��� 2Ԫ������ȡ 2Ԫ������" << endl
		<< "������ȡ�������������Ѹ���50Ԫ������ȡ50Ԫ������" << endl;
	cout << "******************************************************" << endl;
	cout << VipDiscountText(VipLevel);

	double dPoundage = (double)nDrawMoney * 0.01;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	dPoundage *= VipDiscount;
	double dSum = dPoundage + nDrawMoney;
	cout << "ȡ�� " << nDrawMoney << " Ԫ" << endl
		<< "������ " << dPoundage << " Ԫ" << endl
		<< "�۷Ѻϼ� " << dSum << " Ԫ" << endl
		<< "�Ƿ�ȷ��ȡ�����Yȷ�ϡ�" << endl;
	char cSel = getch();
	if (cSel == 'y' || cSel == 'Y') 
	{
		if (Cmd_DrawMontyConfirm2(DrawMoneyCardNumb, dSum, dPoundage) == TRUE)
		{
			cout << "�����³�����Ѹ��ȡ�ߣ�" << endl;
			cout << "ȡ����ϣ�";
			return TRUE;
		}
		else 
		{
			cout << "���㣬�޷����ȡ�" << endl;
			return false;
		}
	}
	else {
		cout << "ȡ��ȡ����" << endl;
	}
	return FALSE;
}

BOOL CATM::Cmd_DrawMontyConfirm2(int DrawMoneyCardNumb, double dSum, double dPoundage)
{
	int nCmd = ATMREQ_DrawMoneyAndPoundage, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&DrawMoneyCardNumb, sizeof(DrawMoneyCardNumb));
	sock.Send(&dSum, sizeof(dSum));
	sock.Send(&dPoundage, sizeof(dPoundage));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}

BOOL CATM::TransferMoney(int nTransferMoneyCardNumb, char sClientName[], int nSex)
{
	//��ʧ������״̬�����п���ֹת��
	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(nTransferMoneyCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
	{
		cout << "�޷�����˿����˿�Ŀǰ�ѱ�" << (nCardSecuritySituation == Freeze ? "����" : "��ʧ") << endl;
		system("pause");
		return FALSE;
	}

	//��Ƭ����״̬��
	system("cls");
	int nToCardNumb;
	cout << "������ת�뿨�ţ�";
	cin >> nToCardNumb;
	if (nTransferMoneyCardNumb == nToCardNumb)
	{
		cout << "�������򱾿�ת�ˣ�" << endl;
		system("pause");
		return FALSE;
	}
	if(!Cmd_isCardNumbExist(nToCardNumb))
	{
		cout << " δ�ҵ�����Ϊ " << nToCardNumb << " �����п���"
			<< " ���ʵ���ٽ���ת�ˣ�" << endl;
		system("pause");
		return FALSE;
	}

	double dTranM;
	cout << endl << "������ת�˽�";
	cin >> dTranM;
	TransferMoneyConfirm(nTransferMoneyCardNumb, nToCardNumb, dTranM);
	system("pause");
}

BOOL CATM::TransferMoneyConfirm(int nTransferMoneyCardNumb, int nToCardNumb, double dTranM)
{
	system("cls");
	int VipLevel = Cmd_GetVipLevel(nTransferMoneyCardNumb);		//��ȡVIP�ȼ�
	double VipDiscount = GetVipDiscount(VipLevel);			//��ȡVIP�ۿ�
	cout << "******************************************************" << endl;
	cout << "�����Ѽ������" << endl
		<< "��������ȡ���� 2% ����Χ�� 2~50Ԫ֮��" << endl
		<< "������ȡ�������������ѵ��� 2Ԫ������ȡ 2Ԫ������" << endl
		<< "������ȡ�������������Ѹ���50Ԫ������ȡ50Ԫ������" << endl;
	cout << "******************************************************" << endl;
	cout << VipDiscountText(VipLevel);

	double dPoundage = dTranM * 0.02;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	dPoundage *= VipDiscount;
	double dSum = dPoundage + dTranM;

	//��ȡת�뿨������Ϣ���ֿ����������Ա�
	BasicData sToCardBasicData;		
	Cmd_GetCardBasicData(nToCardNumb, sToCardBasicData);

	cout<< "ת�� ��" << sToCardBasicData.sName << (sToCardBasicData.nSex == Male ? " ���� " : " Ůʿ ")
		<< "(����Ϊ " << nToCardNumb << " ) "
		<< dTranM << " Ԫ" << endl
		<< "������ " << dPoundage << " Ԫ" << endl
		<< "�۷Ѻϼ� " << dSum << " Ԫ" << endl
		<< "�Ƿ�ȷ��ת�ˣ�����Yȷ�ϡ�" << endl;
	char cSel = getch();
	if (cSel == 'y' || cSel == 'Y')
	{
		if (Cmd_TransferMoneyConfirm2(nTransferMoneyCardNumb, nToCardNumb, dTranM, dPoundage) == TRUE)
		{
			cout << "ת�˳ɹ�,�Է����յ��˿���!" << endl;
			return TRUE;
		}
		else {
			cout << "ת��ʧ�ܣ�ת�������㣡" << endl;
			return FALSE;
		}
	}
	else
	{
		cout << "ת��ȡ����" << endl;
		return FALSE;
	}
}

BOOL CATM::Cmd_TransferMoneyConfirm2(int nTransferMoneyCardNumb, int nToCardNumb, double dTranM, double dPoundage)
{
	int nCmd = ATMREQ_TransferMoney, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nTransferMoneyCardNumb, sizeof(nTransferMoneyCardNumb));
	sock.Send(&nToCardNumb, sizeof(nToCardNumb));
	sock.Send(&dTranM, sizeof(dTranM));
	sock.Send(&dPoundage, sizeof(dPoundage));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}

BOOL CATM::CheckMoney(int nCardNumb, char sClientName[], int nSex)
{
	cout << sClientName
		<< (nSex == Male ? " ����" : "Ůʿ")
		<< "��������Ϊ "
		<< nCardNumb
		<< "�����п����Ϊ "
		<< fixed << setprecision(2) //fixed << serprecision(int x) 
									//��ʹ�ÿ�ѧ��������ʾ�����趨ֻ��ʾxλС��
									//ͷ�ļ�Ϊ <iomanip>
		<< Cmd_CheckSumMoney(nCardNumb)
		<< endl;

	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(nCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
		cout << "�˿�Ŀǰ�ѱ�" << (nCardSecuritySituation == Freeze ? "����" : "��ʧ") << endl;
	system("pause");
	return TRUE;
}

BOOL CATM::ChangePass(int nCardNumb)
{
	char RealOldPass[7] = { 0 };
	strcpy(RealOldPass, Cmd_GetCardPassword(nCardNumb));
	
	system("cls");
	char InputOldPass[7] = { 0 };
	cout << "����������룺";
	int i = 0, Times = 0;
	char ch;
	bool nRes = false;
	do {
		if (Times == 5) {
			Cmd_ApplyFreezeCard(nCardNumb);
			cout << endl << "��������Ѵﵽ���ޣ��˻��ѱ�����!" << endl;
			system("pause");
			return FALSE;
		}
		if (Times > 0) {
			cout << endl << "����ĵ�����������벻�������������룡" << endl
				<< "���������� " << 5 - Times << " ��" << endl;
		}

		i = 0;
		while ((ch = getch()) != '\r'  &&  i < 6)
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
			InputOldPass[i++] = ch;
		}
		InputOldPass[6] = '\0';

		if (strcmp(InputOldPass, RealOldPass) == 0)
			nRes = ChangePassConfirm(nCardNumb);

		Times++;
	} while (nRes != true);

	return false;
}

BOOL CATM::ChangePassConfirm(int nCardNumb)
{
	char SPass[7], CPass[7];
	int nTimes = 0;

	do {
		if (nTimes == 3) {
			Cmd_ApplyFreezeCard(nCardNumb);
			cout << endl << "��������Ѵﵽ���ޣ��޸�����ʧ��!" << endl;
			system("pause");
			return FALSE;
		}

		if (nTimes != 0) {
			cout << endl << "������������벻һ�£�����������" << endl;
			cout << "���������� " << 3 - nTimes << " ��" << endl;
		}
		/********************************/
		//��һ����������
		cout << endl << "�����������룺";
		int i = 0;
		char ch;
		while ((ch = getch()) != '\r'  &&  i < 6)
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
			SPass[i++] = ch;
		}
		SPass[6] = '\0';
		/********************************/
		//�ڶ�����������
		cout << endl << "��������һ�Σ�";
		i = 0;
		while ((ch = getch()) != '\r'  &&  i < 6)
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
			CPass[i++] = ch;
		}
		CPass[6] = '\0';
		/********************************/
		//����ȷ��
		if (strcmp(SPass, CPass) == 0) {
			Cmd_ApplyChangePassword(nCardNumb, CPass);
			cout << endl << "�����޸���ɣ����μ����������룡" << endl;
			system("pause");
			return TRUE;
		}

		++nTimes;
	} while (strcmp(SPass, CPass));
	cout << endl << "�����޸�ʧ�ܣ�" << endl;
	system("pause");
	return FALSE;
}

BOOL CATM::Cmd_ApplyChangePassword(int nCardNumb,char NewPass[])
{
	int nCmd = ATMREQ_ApplyChangePassword, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Send(NewPass, 7);
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}

char* CATM::Cmd_GetCardPassword(int nCardNumb)
{
	int nCmd = ATMREQ_GetCardPassword;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	char sPassword[7] = { 0 };
	sock.Receive(&sPassword, 7);
	return sPassword;
}

BOOL CATM::Cmd_ApplyFreezeCard(int nCardNumb)
{
	int nCmd = ATMREQ_ApplyFreezeCard, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}


BOOL CATM::Cmd_isCardNumbExist(int nLNumb)
{
	int nCmd = ATMREQ_isCardNumbExist, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nLNumb, sizeof(nLNumb));
	sock.Receive(&nRes,sizeof(nRes));
	if (1 == nRes)
		return TRUE;
	else
		return FALSE;
}

BOOL CATM::Cmd_isLoginSucceed(ClientLoginData LoginD,char rClientName[], int &rnSex)
{
	int nCmd = ATMREQ_ClientLogin, nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&LoginD, sizeof(LoginD));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes) {
		//�����½�ɹ����ӷ��������� �ͻ��������ͻ��Ա�
		sock.Receive(rClientName,20);
		sock.Receive(&rnSex, sizeof(rnSex));
		return TRUE;
	}	
	else
		return FALSE;
}

int CATM::Cmd_ApplyNewCardNumb()
{
	int nCmd = ATMREQ_ApplyNewCardNumb, NewCardNumb = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Receive(&NewCardNumb, sizeof(NewCardNumb));
	return NewCardNumb;
}

BOOL CATM::Cmd_SendNewApplyAccountInfo(ClientData &NewData)
{
	int nCmd = ATMREQ_CreateNewAccount, NewCardNumb = 0 ,nRes = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&NewData, sizeof(ClientData));
	sock.Receive(&nRes, sizeof(nRes));
	if(1 == nRes)
		return TRUE;
	return FALSE;
}


int CATM::Cmd_AppleCardSecuritySituation(int nCardNumb)
{
	int nCmd = ATMREQ_AppleCardSecuritySituation, nSecuritySituation = 0;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Receive(&nSecuritySituation, sizeof(nSecuritySituation));
	return nSecuritySituation;
}

BOOL CATM::Cmd_GetCardBasicData(int nCardNumb, BasicData &rBasicData)
{
	int nCmd = ATMREQ_ApplyCardBasicData, nRes;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Receive(&nRes, sizeof(nRes));
	if (1 == nRes) {
		sock.Receive(&rBasicData.nSex, sizeof(rBasicData.nSex));
		sock.Receive(&rBasicData.sName, 20);
		return TRUE;
	}
	return FALSE;
}

BOOL CATM::Cmd_ApplyDepositMoney(int nDepositCardNumb, int nDepositMoney)
{
	int nCmd = ATMREQ_ApplyDepositMoney, nSecuritySituation = 0, nRes;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nDepositCardNumb, sizeof(nDepositCardNumb));
	sock.Send(&nDepositMoney, sizeof(nDepositMoney));
	sock.Receive(&nRes, sizeof(nRes));
	if(1 == nRes)
		return TRUE;
	return FALSE;
}


double CATM::Cmd_CheckSumMoney(int nCardNumb)
{
	int nCmd = ATMREQ_CheckSumMoney;
	double dSumMoney;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Receive(&dSumMoney, sizeof(dSumMoney));
	return dSumMoney;
}


int CATM::Cmd_GetVipLevel(int nCardNumb)
{
	int nCmd = ATMREQ_GetVipLevel;
	int nResult;
	SockJollyEx sock;
	sock.Send(&nCmd, sizeof(nCmd));
	sock.Send(&nCardNumb, sizeof(nCardNumb));
	sock.Receive(&nResult, sizeof(nResult));
	return nResult;
}

double CATM::GetVipDiscount(int nVipLevel)
{
	switch (nVipLevel)
	{
	case VIP1:
		return 0.9;
		break;
	case VIP2:
		return 0.8;
		break;
	case VIP3:
		return 0.7;
		break;
	case VIP4:
		return 0.6;
		break;
	case VIP5:
		return 0.5;
		break;
	default:
		return 1.0;
		break;
	}
}

char* CATM::Time()
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

char* CATM::VipDiscountText(int VipLevel)
{
	switch (VipLevel)
	{
	case VIP1:
		return "******�������� VIP1 ���ͻ������������� 9 ���ۿ�*****\n";
		break;
	case VIP2:
		return "******�������� VIP2 ���ͻ������������� 8 ���ۿ�*****\n";
		break;
	case VIP3:
		return "******�������� VIP3 ���ͻ������������� 7 ���ۿ�*****\n";
		break;
	case VIP4:
		return "******�������� VIP4 ���ͻ������������� 6 ���ۿ�*****\n";
		break;
	case VIP5:
		return "******�������� VIP5 ���ͻ������������� 5 ���ۿ�*****\n";
		break;
	default:
		return '\0';
	}
}