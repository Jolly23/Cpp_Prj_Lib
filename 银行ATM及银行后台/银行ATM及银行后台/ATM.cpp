// ATM.cpp: implementation of the CATM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ATM.h"
#include "Client.h"
using namespace std;
extern vector<ClientData> ClientList;
enum CardSecurity { Normal = 486, Freeze, Loss };
enum VIP { VIP0 = 4894613, VIP1, VIP2, VIP3, VIP4, VIP5, VIP6, Abnormal };
enum Sex { Male = 10086, Female };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CATM::CATM()
{
	int P = m_Client.Login();
	if (P == -1)
		return;
	while (ATMMenu(P));
}

CATM::~CATM()
{

}


int CATM::ATMMenu(int p_Client)
{
	system("cls");
	cout << "******************************************" << endl;
	cout << "*********�𾴵� " << ClientList[p_Client].sName
		<< (ClientList[p_Client].nSex == Male ? " ����," : " Ůʿ,")
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
		this->Deposit(p_Client);
		break;
	case '2':
		this->DrawMoney(p_Client);
		break;
	case '3':
		this->TransferMoney(p_Client);
		break;
	case '4':
		this->CheckMoney(p_Client);
		break;
	case '5':
		this->ChangePass(p_Client);
		break;
	case '0':
		return 0;
	};
	return 1;
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

bool CATM::Deposit(int p_Client)
{
	system("cls");

	//��ʧ������״̬�����п���ֹ��Ǯ
	if (ClientList[p_Client].nCardSecurity == Freeze || ClientList[p_Client].nCardSecurity == Loss)
	{
		cout << "�޷�����˿����˿�Ŀǰ�ѱ�" << (ClientList[p_Client].nCardSecurity == Freeze ? "����" : "��ʧ") << endl;
		system("pause");
		return false;
	}

	//��Ƭ����״̬��
	int nSizeMoney;
	do {
		cout << "�����������(100�ı���)��" << endl;
		cin >> nSizeMoney;
	} while (nSizeMoney % 100 != 0);

	double DepositMoney = 100.0 * (double)nSizeMoney;
	ClientList[p_Client].dMoney += DepositMoney;
	cout << "������ɣ�" << endl
		<< ClientList[p_Client].sName
		<< (ClientList[p_Client].nSex == Male ? " ����" : "Ůʿ")
		<< "��������Ϊ "
		<< ClientList[p_Client].nCardNumb
		<< "�����п����Ϊ "
		<< fixed << setprecision(2) << ClientList[p_Client].dMoney
		<< endl;

	ClientList[p_Client].nVIP = m_Client.isVIP(ClientList[p_Client].nCardNumb);
	m_Client.Save();
	system("pause");
	return true;
}



bool CATM::DrawMoney(int p_Client)
{
	system("cls");

	//��ʧ������״̬�����п���ֹȡ��
	if (ClientList[p_Client].nCardSecurity == Freeze || ClientList[p_Client].nCardSecurity == Loss)
	{
		cout << "�޷�ȡ��˿�Ŀǰ�ѱ�" << (ClientList[p_Client].nCardSecurity == Freeze ? "����" : "��ʧ") << endl;
		system("pause");
		return false;
	}

	//��Ƭ����״̬��
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
	if (DrawMontyConfirm(p_Client, nDrawMoney) == true)
		return true;
	else
		return false;
}

bool CATM::DrawMontyConfirm(int p_Client, int nDrawMoney)
{
	system("cls");
	cout << "******************************************************" << endl;
	cout << "�����Ѽ������" << endl
		<< "��������ȡ���� 1% ����Χ�� 2~50Ԫ֮��" << endl
		<< "������ȡ�������������ѵ��� 2Ԫ������ȡ 2Ԫ������" << endl
		<< "������ȡ�������������Ѹ���50Ԫ������ȡ50Ԫ������" << endl;
	cout << "******************************************************" << endl;

	double dPoundage = (double)nDrawMoney * 0.01;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	double dSum = dPoundage + nDrawMoney;
	cout << "ȡ�� " << nDrawMoney << " Ԫ" << endl
		<< "������ " << dPoundage << " Ԫ" << endl
		<< "�۷Ѻϼ� " << dSum << " Ԫ" << endl
		<< "�Ƿ�ȷ��ȡ�����Yȷ�ϡ�" << endl;
	char cSel = getch();
	if (cSel == 'y' || cSel == 'Y')
		if (DrawMontyConfirm2(p_Client, dSum) == true) {
			ClientList[p_Client].nPayForBank += dPoundage;
			return true;
		}
		else
			return false;
	return false;
}

bool CATM::DrawMontyConfirm2(int p_Client, double dDrawMoney)
{
	ClientData &Client = ClientList[p_Client];
	if (Client.dMoney >= dDrawMoney)
	{
		Client.dMoney -= dDrawMoney;
		Client.nVIP = m_Client.isVIP(Client.nCardNumb);			//VIP�ȼ������ж�
		m_Client.Save();
		cout << "�����³�����Ѹ��ȡ�ߣ�" << endl;
		cout << "ȡ����ϣ�";
		system("pause");
		return true;
	}
	else
	{
		cout << "���㣬�޷����ȡ�" << endl;
		system("pause");
		return false;
	}
}

void CATM::CheckMoney(int p_Client)
{
	cout << "����Ϊ " << ClientList[p_Client].nCardNumb << " �����Ϊ "
		<< fixed << setprecision(2) //fixed << serprecision(int x) ��ʹ�ÿ�ѧ��������ʾ�����趨ֻ��ʾ2λС��
									//ͷ�ļ�Ϊ <iomanip>
		<< ClientList[p_Client].dMoney << " Ԫ" << endl;
	system("pause");
}

void CATM::TransferMoney(int p_Client)
{
	system("cls");

	//��ʧ������״̬�����п���ֹת��
	if (ClientList[p_Client].nCardSecurity == Freeze || ClientList[p_Client].nCardSecurity == Loss)
	{
		cout << "�޷�ת�ˣ��˿�Ŀǰ�ѱ�" << (ClientList[p_Client].nCardSecurity == Freeze ? "����" : "��ʧ") << endl;
		system("pause");
		return;
	}

	//��Ƭ����״̬��
	int nToCardNumb;
	cout << "������ת�뿨�ţ�";
	cin >> nToCardNumb;
	if (ClientList[p_Client].nCardNumb == nToCardNumb)
	{
		cout << "�������򱾿�ת�ˣ�" << endl;
		system("pause");
		return;
	}
	int pTo = m_Client.CheckCardNumb(nToCardNumb);
	if (pTo == -1)
	{
		cout << " δ�ҵ�����Ϊ " << nToCardNumb << " �����п���"
			<< " ���ʵ���ٽ���ת�ˣ�" << endl;
		system("pause");
		return;
	}
	ClientData &ToCard = ClientList[pTo];

	double dTranM;
	cout << endl << "������ת�˽�";
	cin >> dTranM;
	TransferMoneyConfirm(p_Client, &ToCard, dTranM);
}

bool CATM::TransferMoneyConfirm(int p_Client, ClientData *ToCard, double dTranM)
{
	system("cls");
	cout << "******************************************************" << endl;
	cout << "�����Ѽ������" << endl
		<< "��������ȡ���� 2% ����Χ�� 2~50Ԫ֮��" << endl
		<< "������ȡ�������������ѵ��� 2Ԫ������ȡ 2Ԫ������" << endl
		<< "������ȡ�������������Ѹ���50Ԫ������ȡ50Ԫ������" << endl;
	cout << "******************************************************" << endl;

	double dPoundage = dTranM * 0.02;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	double dSum = dPoundage + dTranM;
	cout << "ת�� ��" << ToCard->sName << (ToCard->nSex == Male ? " ���� " : " Ůʿ ")
		<< "(����Ϊ " << ToCard->nCardNumb << " ) "
		<< dTranM << " Ԫ" << endl
		<< "������ " << dPoundage << " Ԫ" << endl
		<< "�۷Ѻϼ� " << dSum << " Ԫ" << endl
		<< "�Ƿ�ȷ��ת�ˣ�����Yȷ�ϡ�" << endl;
	char cSel = getch();
	if (cSel == 'y' || cSel == 'Y')
		if (TransferMoneyConfirm2(p_Client, ToCard, dTranM, dSum) == true) {
			ClientList[p_Client].nPayForBank += dPoundage;
			return true;
		}		
		else
			return false;
	else
	{
		cout << "ת��ȡ����" << endl;
		system("pause");
		return false;
	}
}

bool CATM::TransferMoneyConfirm2(int p_Client, ClientData *ToCard, double dTranM, double dSum)
{
	ClientData &FrCard = ClientList[p_Client];
	if (FrCard.dMoney >= dSum)
	{
		FrCard.dMoney -= dSum;
		ToCard->dMoney += dTranM;
		cout << endl << "ת�˳ɹ����Է����յ����Ŀ��" << endl;
		FrCard.nVIP = m_Client.isVIP(FrCard.nCardNumb);
		ToCard->nVIP = m_Client.isVIP(ToCard->nCardNumb);
		m_Client.Save();
		system("pause");
		return true;
	}
	else
	{
		cout << endl << "ת��ʧ�ܣ�ת�������㣡" << endl;
		system("pause");
		return false;
	}
}

bool CATM::ChangePass(int p_Client)
{
	ClientData &Client = ClientList[p_Client];
	system("cls");
	cout << "����������룺";
	char OldPass[7];
	int i = 0, Times = 0;
	char ch;
	bool nRes = false;
	do {
		if (Times == 3) {
			cout << endl << "��������Ѵﵽ���ޣ��޸�������ֹ!" << endl;
			system("pause");
			return false;
		}
		if (Times > 0) {
			cout << endl << "����ĵ�����������벻�������������룡" << endl
				<< "���������� " << 3 - Times << " ��" << endl;
		}

		i = 0;
		while ((ch = getch()) != '\r'  &&  i < 6)
		{
			if (ch == '\b') {
				--i;
				cout << '\b';
				continue;
			}
			putchar('*');
			OldPass[i++] = ch;
		}
		OldPass[6] = '\0';

		if (strcmp(OldPass, Client.sPass) == 0)
			nRes = ChangePassConfirm(&Client);

		Times++;
	} while (nRes != true);

	return false;
}

bool CATM::ChangePassConfirm(ClientData *Client)
{
	char SPass[7], CPass[7];
	int nTimes = 0;

	do {
		if (nTimes != 0)
			cout << endl << "������������벻һ�£�����������" << endl;
		/********************************/
		//��һ����������
		cout << endl << "�����������룺";
		int i = 0;
		char ch;
		while ((ch = getch()) != '\r'  &&  i < 6)
		{
			if (ch == '\b') {
				--i;
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
			if (ch == '\b') {
				--i;
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
			strcpy(Client->sPass, SPass);
		++nTimes;
	} while (strcmp(SPass, CPass));
	m_Client.Save();
	cout << endl << "�����޸���ɣ����μ����������룡" << endl;
	system("pause");
	return true;
}
