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
	cout << "请输入您的姓名：";
	cin >> pData->sName;
	char cSel;
	do {
		cout << "请选择性别" << endl
			<< "1. 男" << endl
			<< "2. 女" << endl;
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
			cout << endl << "两次输入的密码不一致，请重新输入" << endl;
		/********************************/
		//第一次输入密码
		cout << endl << "请设定密码：";
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
		//第二次输入密码
		cout << endl << "请再输入一次：";
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
		//密码确认
		if (strcmp(SPass, CPass) == 0)
			strcpy(pData->sPass, SPass);
		++nTimes;
	} while (strcmp(SPass, CPass));

	//向服务器申请卡号：
	pData->nCardNumb = Cmd_ApplyNewCardNumb();
	//录入基础数据
	pData->dMoney = 0.0;
	pData->dOverdraft = 0.0;
	pData->dOverdraftMoney = 0.0;
	pData->nVIP = VIP0;
	pData->nCardSecurity = Normal;
	pData->dPayForBank = 0.0;

	//向服务器发送新客户信息
	Cmd_SendNewApplyAccountInfo(*pData);

	cout << endl << endl
		<< "欢迎 " << pData->sName << (pData->nSex == Male ? " 先生" : " 女士") << "成为招商银行客户!" << endl
		<< "您的银行卡登录用户名(卡号)为 " << pData->nCardNumb << endl;
	delete pData;

	system("pause");
}

int CATM::Login()
{
	system("cls");
	ClientLoginData LoginD;

	cout << "请输入登录卡号：";
	cin >> LoginD.nCardNumb;

	if (!Cmd_isCardNumbExist(LoginD.nCardNumb))
	{
		cout << "您输入的卡号不存在！" << endl;
		system("pause");
		return -1;
	}

	cout << endl << "请输入密码：";
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
		cout << endl << "登陆失败！" << endl;
		system("pause");
		return -1;
	}
	return 1;
}

int CATM::ATMMenu(int ClientCardNumb, char sClientName[], int nSex)
{
	system("cls");
	cout << "******************************************" << endl;
	cout << "*********尊敬的 " << sClientName
		<< (nSex == Male ? " 先生," : " 女士,")
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
	//挂失、冻结状态，银行卡禁止存钱
	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(DepositCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
	{
		cout << "无法存入此卡，此卡目前已被" << (nCardSecuritySituation == Freeze ? "冻结" : "挂失") << endl;
		system("pause");
		return FALSE;
	}
	
	//卡片正常状态：
	int nDepositMoney = 0;
	do {
		cout << "请输入存入金额(100的倍数)：";
		cin >> nDepositMoney;
	} while (nDepositMoney % 100 != 0  && nDepositMoney == 0);

	Cmd_ApplyDepositMoney(DepositCardNumb, nDepositMoney);

	cout<< "存入完成！" << endl
		<< sClientName
		<< (nSex == Male ? " 先生" : "女士")
		<< "，您卡号为 "
		<< DepositCardNumb
		<< "的银行卡余额为 "
		<< Cmd_CheckSumMoney(DepositCardNumb)
		<< endl;

	system("pause");
	return TRUE;
}

BOOL CATM::DrawMoney(int DrawMoneyCardNumb, char sClientName[], int nSex)
{
	//挂失、冻结状态，银行卡禁止取钱
	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(DrawMoneyCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
	{
		cout << "无法存入此卡，此卡目前已被" << (nCardSecuritySituation == Freeze ? "冻结" : "挂失") << endl;
		system("pause");
		return FALSE;
	}

	//卡片正常状态：
	system("cls");
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
	DrawMontyConfirm(DrawMoneyCardNumb, nDrawMoney);
	CheckMoney(DrawMoneyCardNumb, sClientName,nSex);

	return TRUE;
}

BOOL CATM::DrawMontyConfirm(int DrawMoneyCardNumb, int nDrawMoney)
{
	system("cls");

	int VipLevel = Cmd_GetVipLevel(DrawMoneyCardNumb);			//获取VIP等级
	double VipDiscount = GetVipDiscount(VipLevel);			//获取VIP折扣
	
	cout << "******************************************************" << endl;
	cout << "手续费计算规则：" << endl
		<< "手续费是取款额的 1% ，范围在 2~50元之间" << endl
		<< "若本次取款按计算规则手续费低于 2元，则收取 2元手续费" << endl
		<< "若本次取款按计算规则手续费高于50元，则收取50元手续费" << endl;
	cout << "******************************************************" << endl;
	cout << VipDiscountText(VipLevel);

	double dPoundage = (double)nDrawMoney * 0.01;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	dPoundage *= VipDiscount;
	double dSum = dPoundage + nDrawMoney;
	cout << "取款 " << nDrawMoney << " 元" << endl
		<< "手续费 " << dPoundage << " 元" << endl
		<< "扣费合计 " << dSum << " 元" << endl
		<< "是否确认取款？【按Y确认】" << endl;
	char cSel = getch();
	if (cSel == 'y' || cSel == 'Y') 
	{
		if (Cmd_DrawMontyConfirm2(DrawMoneyCardNumb, dSum, dPoundage) == TRUE)
		{
			cout << "正在吐钞，请迅速取走！" << endl;
			cout << "取款完毕！";
			return TRUE;
		}
		else 
		{
			cout << "余额不足，无法完成取款！" << endl;
			return false;
		}
	}
	else {
		cout << "取款取消！" << endl;
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
	//挂失、冻结状态，银行卡禁止转账
	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(nTransferMoneyCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
	{
		cout << "无法存入此卡，此卡目前已被" << (nCardSecuritySituation == Freeze ? "冻结" : "挂失") << endl;
		system("pause");
		return FALSE;
	}

	//卡片正常状态：
	system("cls");
	int nToCardNumb;
	cout << "请输入转入卡号：";
	cin >> nToCardNumb;
	if (nTransferMoneyCardNumb == nToCardNumb)
	{
		cout << "不允许向本卡转账！" << endl;
		system("pause");
		return FALSE;
	}
	if(!Cmd_isCardNumbExist(nToCardNumb))
	{
		cout << " 未找到卡号为 " << nToCardNumb << " 的银行卡！"
			<< " 请查实后再进行转账！" << endl;
		system("pause");
		return FALSE;
	}

	double dTranM;
	cout << endl << "请输入转账金额：";
	cin >> dTranM;
	TransferMoneyConfirm(nTransferMoneyCardNumb, nToCardNumb, dTranM);
	system("pause");
}

BOOL CATM::TransferMoneyConfirm(int nTransferMoneyCardNumb, int nToCardNumb, double dTranM)
{
	system("cls");
	int VipLevel = Cmd_GetVipLevel(nTransferMoneyCardNumb);		//获取VIP等级
	double VipDiscount = GetVipDiscount(VipLevel);			//获取VIP折扣
	cout << "******************************************************" << endl;
	cout << "手续费计算规则：" << endl
		<< "手续费是取款额的 2% ，范围在 2~50元之间" << endl
		<< "若本次取款按计算规则手续费低于 2元，则收取 2元手续费" << endl
		<< "若本次取款按计算规则手续费高于50元，则收取50元手续费" << endl;
	cout << "******************************************************" << endl;
	cout << VipDiscountText(VipLevel);

	double dPoundage = dTranM * 0.02;
	if (dPoundage > 50.0)
		dPoundage = 50.0;
	if (dPoundage < 2.0)
		dPoundage = 2.0;
	dPoundage *= VipDiscount;
	double dSum = dPoundage + dTranM;

	//获取转入卡基本信息（持卡人姓名、性别）
	BasicData sToCardBasicData;		
	Cmd_GetCardBasicData(nToCardNumb, sToCardBasicData);

	cout<< "转账 给" << sToCardBasicData.sName << (sToCardBasicData.nSex == Male ? " 先生 " : " 女士 ")
		<< "(卡号为 " << nToCardNumb << " ) "
		<< dTranM << " 元" << endl
		<< "手续费 " << dPoundage << " 元" << endl
		<< "扣费合计 " << dSum << " 元" << endl
		<< "是否确认转账？【按Y确认】" << endl;
	char cSel = getch();
	if (cSel == 'y' || cSel == 'Y')
	{
		if (Cmd_TransferMoneyConfirm2(nTransferMoneyCardNumb, nToCardNumb, dTranM, dPoundage) == TRUE)
		{
			cout << "转账成功,对方已收到此款项!" << endl;
			return TRUE;
		}
		else {
			cout << "转账失败，转出卡余额不足！" << endl;
			return FALSE;
		}
	}
	else
	{
		cout << "转账取消！" << endl;
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
		<< (nSex == Male ? " 先生" : "女士")
		<< "，您卡号为 "
		<< nCardNumb
		<< "的银行卡余额为 "
		<< fixed << setprecision(2) //fixed << serprecision(int x) 
									//不使用科学计数法显示，并设定只显示x位小数
									//头文件为 <iomanip>
		<< Cmd_CheckSumMoney(nCardNumb)
		<< endl;

	int nCardSecuritySituation = Cmd_AppleCardSecuritySituation(nCardNumb);
	if (nCardSecuritySituation == Freeze || nCardSecuritySituation == Loss)
		cout << "此卡目前已被" << (nCardSecuritySituation == Freeze ? "冻结" : "挂失") << endl;
	system("pause");
	return TRUE;
}

BOOL CATM::ChangePass(int nCardNumb)
{
	char RealOldPass[7] = { 0 };
	strcpy(RealOldPass, Cmd_GetCardPassword(nCardNumb));
	
	system("cls");
	char InputOldPass[7] = { 0 };
	cout << "请输入旧密码：";
	int i = 0, Times = 0;
	char ch;
	bool nRes = false;
	do {
		if (Times == 5) {
			Cmd_ApplyFreezeCard(nCardNumb);
			cout << endl << "输入次数已达到上限，账户已被冻结!" << endl;
			system("pause");
			return FALSE;
		}
		if (Times > 0) {
			cout << endl << "输入的的密码与旧密码不符，请重新输入！" << endl
				<< "您还能输入 " << 5 - Times << " 次" << endl;
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
			cout << endl << "输入次数已达到上限，修改密码失败!" << endl;
			system("pause");
			return FALSE;
		}

		if (nTimes != 0) {
			cout << endl << "两次输入的密码不一致，请重新输入" << endl;
			cout << "您还能输入 " << 3 - nTimes << " 次" << endl;
		}
		/********************************/
		//第一次输入密码
		cout << endl << "请输入新密码：";
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
		//第二次输入密码
		cout << endl << "请再输入一次：";
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
		//密码确认
		if (strcmp(SPass, CPass) == 0) {
			Cmd_ApplyChangePassword(nCardNumb, CPass);
			cout << endl << "密码修改完成，请牢记您的新密码！" << endl;
			system("pause");
			return TRUE;
		}

		++nTimes;
	} while (strcmp(SPass, CPass));
	cout << endl << "密码修改失败！" << endl;
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
		//如果登陆成功，从服务器接收 客户姓名、客户性别
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
		strcpy(pChar, "晚上好");
	if (Hours >= 5 && Hours < 11)
		strcpy(pChar, "上午好");
	if (Hours >= 11 && Hours < 13)
		strcpy(pChar, "中午好");
	if (Hours >= 13 && Hours < 19)
		strcpy(pChar, "下午好");
	return pChar;
}

char* CATM::VipDiscountText(int VipLevel)
{
	switch (VipLevel)
	{
	case VIP1:
		return "******你是尊贵的 VIP1 级客户，享受手续费 9 折折扣*****\n";
		break;
	case VIP2:
		return "******你是尊贵的 VIP2 级客户，享受手续费 8 折折扣*****\n";
		break;
	case VIP3:
		return "******你是尊贵的 VIP3 级客户，享受手续费 7 折折扣*****\n";
		break;
	case VIP4:
		return "******你是尊贵的 VIP4 级客户，享受手续费 6 折折扣*****\n";
		break;
	case VIP5:
		return "******你是尊贵的 VIP5 级客户，享受手续费 5 折折扣*****\n";
		break;
	default:
		return '\0';
	}
}