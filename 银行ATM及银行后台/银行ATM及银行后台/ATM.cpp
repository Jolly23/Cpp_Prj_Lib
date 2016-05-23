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
	cout << "*********尊敬的 " << ClientList[p_Client].sName
		<< (ClientList[p_Client].nSex == Male ? " 先生," : " 女士,")
		<< Time() << "********" << endl;
	cout << "******************************************" << endl;
	cout << "**  1.存款                      取款.2  **" << endl
		<< "**  3.转账                  查询余额.4  **" << endl
		<< "**  5.修改密码            信用卡还款.6  **" << endl
		<< "**                0.退卡                **" << endl;
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
		strcpy(pChar, "晚上好");
	if (Hours >= 5 && Hours < 11)
		strcpy(pChar, "上午好");
	if (Hours >= 11 && Hours < 13)
		strcpy(pChar, "中午好");
	if (Hours >= 13 && Hours < 19)
		strcpy(pChar, "下午好");
	return pChar;
}

bool CATM::Deposit(int p_Client)
{
	system("cls");

	//挂失、冻结状态，银行卡禁止存钱
	if (ClientList[p_Client].nCardSecurity == Freeze || ClientList[p_Client].nCardSecurity == Loss)
	{
		cout << "无法存入此卡，此卡目前已被" << (ClientList[p_Client].nCardSecurity == Freeze ? "冻结" : "挂失") << endl;
		system("pause");
		return false;
	}

	//卡片正常状态：
	int nSizeMoney;
	do {
		cout << "请输入存入金额(100的倍数)：" << endl;
		cin >> nSizeMoney;
	} while (nSizeMoney % 100 != 0);

	double DepositMoney = 100.0 * (double)nSizeMoney;
	ClientList[p_Client].dMoney += DepositMoney;
	cout << "存入完成！" << endl
		<< ClientList[p_Client].sName
		<< (ClientList[p_Client].nSex == Male ? " 先生" : "女士")
		<< "，您卡号为 "
		<< ClientList[p_Client].nCardNumb
		<< "的银行卡余额为 "
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

	//挂失、冻结状态，银行卡禁止取款
	if (ClientList[p_Client].nCardSecurity == Freeze || ClientList[p_Client].nCardSecurity == Loss)
	{
		cout << "无法取款，此卡目前已被" << (ClientList[p_Client].nCardSecurity == Freeze ? "冻结" : "挂失") << endl;
		system("pause");
		return false;
	}

	//卡片正常状态：
	int nDrawMoney;

	cout << "******************************************" << endl;
	cout << "**  1.--100元               1000元--.4  **" << endl
		<< "**  2.--300元               3000元--.5  **" << endl
		<< "**  3.--500元             输入金额--.6  **" << endl
		<< "**                0.取消                **" << endl;
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
			cout << "请输入取款金额(100的倍数)：" << endl;
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
	cout << "手续费计算规则：" << endl
		<< "手续费是取款额的 1% ，范围在 2~50元之间" << endl
		<< "若本次取款按计算规则手续费低于 2元，则收取 2元手续费" << endl
		<< "若本次取款按计算规则手续费高于50元，则收取50元手续费" << endl;
	cout << "******************************************************" << endl;

	double dPoundage = (double)nDrawMoney * 0.01;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	double dSum = dPoundage + nDrawMoney;
	cout << "取款 " << nDrawMoney << " 元" << endl
		<< "手续费 " << dPoundage << " 元" << endl
		<< "扣费合计 " << dSum << " 元" << endl
		<< "是否确认取款？【按Y确认】" << endl;
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
		Client.nVIP = m_Client.isVIP(Client.nCardNumb);			//VIP等级重新判断
		m_Client.Save();
		cout << "正在吐钞，请迅速取走！" << endl;
		cout << "取款完毕！";
		system("pause");
		return true;
	}
	else
	{
		cout << "余额不足，无法完成取款！" << endl;
		system("pause");
		return false;
	}
}

void CATM::CheckMoney(int p_Client)
{
	cout << "卡号为 " << ClientList[p_Client].nCardNumb << " 的余额为 "
		<< fixed << setprecision(2) //fixed << serprecision(int x) 不使用科学计数法显示，并设定只显示2位小数
									//头文件为 <iomanip>
		<< ClientList[p_Client].dMoney << " 元" << endl;
	system("pause");
}

void CATM::TransferMoney(int p_Client)
{
	system("cls");

	//挂失、冻结状态，银行卡禁止转账
	if (ClientList[p_Client].nCardSecurity == Freeze || ClientList[p_Client].nCardSecurity == Loss)
	{
		cout << "无法转账，此卡目前已被" << (ClientList[p_Client].nCardSecurity == Freeze ? "冻结" : "挂失") << endl;
		system("pause");
		return;
	}

	//卡片正常状态：
	int nToCardNumb;
	cout << "请输入转入卡号：";
	cin >> nToCardNumb;
	if (ClientList[p_Client].nCardNumb == nToCardNumb)
	{
		cout << "不允许向本卡转账！" << endl;
		system("pause");
		return;
	}
	int pTo = m_Client.CheckCardNumb(nToCardNumb);
	if (pTo == -1)
	{
		cout << " 未找到卡号为 " << nToCardNumb << " 的银行卡！"
			<< " 请查实后再进行转账！" << endl;
		system("pause");
		return;
	}
	ClientData &ToCard = ClientList[pTo];

	double dTranM;
	cout << endl << "请输入转账金额：";
	cin >> dTranM;
	TransferMoneyConfirm(p_Client, &ToCard, dTranM);
}

bool CATM::TransferMoneyConfirm(int p_Client, ClientData *ToCard, double dTranM)
{
	system("cls");
	cout << "******************************************************" << endl;
	cout << "手续费计算规则：" << endl
		<< "手续费是取款额的 2% ，范围在 2~50元之间" << endl
		<< "若本次取款按计算规则手续费低于 2元，则收取 2元手续费" << endl
		<< "若本次取款按计算规则手续费高于50元，则收取50元手续费" << endl;
	cout << "******************************************************" << endl;

	double dPoundage = dTranM * 0.02;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	double dSum = dPoundage + dTranM;
	cout << "转账 给" << ToCard->sName << (ToCard->nSex == Male ? " 先生 " : " 女士 ")
		<< "(卡号为 " << ToCard->nCardNumb << " ) "
		<< dTranM << " 元" << endl
		<< "手续费 " << dPoundage << " 元" << endl
		<< "扣费合计 " << dSum << " 元" << endl
		<< "是否确认转账？【按Y确认】" << endl;
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
		cout << "转账取消！" << endl;
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
		cout << endl << "转账成功！对方已收到您的款项！" << endl;
		FrCard.nVIP = m_Client.isVIP(FrCard.nCardNumb);
		ToCard->nVIP = m_Client.isVIP(ToCard->nCardNumb);
		m_Client.Save();
		system("pause");
		return true;
	}
	else
	{
		cout << endl << "转账失败！转出卡余额不足！" << endl;
		system("pause");
		return false;
	}
}

bool CATM::ChangePass(int p_Client)
{
	ClientData &Client = ClientList[p_Client];
	system("cls");
	cout << "请输入旧密码：";
	char OldPass[7];
	int i = 0, Times = 0;
	char ch;
	bool nRes = false;
	do {
		if (Times == 3) {
			cout << endl << "输入次数已达到上限，修改密码终止!" << endl;
			system("pause");
			return false;
		}
		if (Times > 0) {
			cout << endl << "输入的的密码与旧密码不符，请重新输入！" << endl
				<< "您还能输入 " << 3 - Times << " 次" << endl;
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
			cout << endl << "两次输入的密码不一致，请重新输入" << endl;
		/********************************/
		//第一次输入密码
		cout << endl << "请输入新密码：";
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
		//第二次输入密码
		cout << endl << "请再输入一次：";
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
		//密码确认
		if (strcmp(SPass, CPass) == 0)
			strcpy(Client->sPass, SPass);
		++nTimes;
	} while (strcmp(SPass, CPass));
	m_Client.Save();
	cout << endl << "密码修改完成，请牢记您的新密码！" << endl;
	system("pause");
	return true;
}
