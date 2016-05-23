// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Client.h"
using namespace std;
vector<ClientData> ClientList;
enum CardSecurity { Normal = 486, Freeze, Loss };
enum VIP { VIP0 = 4894613, VIP1, VIP2, VIP3, VIP4, VIP5, VIP6, Abnormal };
enum Sex { Male = 10086, Female };
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient()
{
	this->Load();

	if (CheckCardNumb(1001) == -1) {
		ClientData *pData = new ClientData;
		pData->nCardNumb = 1001;
		strcpy(pData->sName, "董事长");
		pData->nSex = Male;
		strcpy(pData->sPass, "123456");
		pData->dMoney = 0.0;
		pData->dOverdraft = 100000.0;
		pData->dOverdraftMoney = 0.0;
		pData->nPayForBank = 0.0;
		pData->nCardSecurity = Normal;
		pData->nVIP = VIP6;

		ClientList.push_back(*pData);
		delete[]pData;
	}
}

CClient::~CClient()
{
	this->Save();
}

int CClient::CheckCardNumb(int Numb)
{
	int i = 0;
	int nCount = ClientList.size();
	while (i < nCount)
	{
		if (ClientList[i].nCardNumb == Numb)
			return i;
		++i;
	}
	return -1;
}

int CClient::Login()
{
	system("cls");

	/***************************************/
	/***************************************/
	///////////////////////////////////
	int iii = 0;
	int nnnCount = ClientList.size();
	while (iii < nnnCount)
	{
		cout << iii + 1 << "：" << ClientList[iii].sName << "\t" << ClientList[iii].nCardNumb << "\t"
			<< ClientList[iii].sPass << "\t给银行的收入=" << ClientList[iii].nPayForBank << endl;
		++iii;
	}
	///////////////////////////////////
	/***************************************/
	/***************************************/

	int nCount = ClientList.size();

	int sLNumb;
	cout << "请输入登录卡号：";
	cin >> sLNumb;

	int nResult = CheckCardNumb(sLNumb);

	if (nResult == -1) {
		cout << endl << "卡号输入有误！" << endl;
		system("pause");
		return -1;
	}

	char sLPass[7];
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
		sLPass[i++] = ch;
	}
	sLPass[6] = '\0';

	if (strcmp(sLPass, ClientList[nResult].sPass) == 0)
		return nResult;
	cout << endl << "密码错误！" << endl;
	system("pause");
	return -1;

}

void CClient::Save()
{
	FILE* fp = fopen("./CMBCClient.lv", "wb");
	if (!fp)
	{
		cout << "保存文件时失败！" << endl;
		return;
	}
	int i = 0;
	int nCount = ClientList.size();
	while (i < nCount)//内存->文件:依据循环到链表结尾
	{
		fwrite(&ClientList[i], 1, sizeof(ClientData), fp);//p ->data类型是struct SStud类型	
		++i;
	}
	fclose(fp);
}

void CClient::Load()
{
	ClientList.clear();
	FILE* fp = fopen("./CMBCClient.lv", "rb");
	if (!fp)
		return;
	ClientData data;
	while (fread(&data, 1, sizeof(ClientData), fp)>0)
		ClientList.push_back(data);
	fclose(fp);
}

int CClient::isVIP(int CNumb)
{
	int P = CheckCardNumb(CNumb);

	if (P = -1)
		return Abnormal;
	if (ClientList[P].nCardSecurity == Freeze)
		return Abnormal;
	if (ClientList[P].nCardSecurity == Loss)
		return Abnormal;

	if (ClientList[P].dMoney <= 5.0)
		return VIP1;
	if (ClientList[P].dMoney <= 100.0)
		return VIP2;
	if (ClientList[P].dMoney <= 1000.0)
		return VIP3;
	if (ClientList[P].dMoney <= 10000.0)
		return VIP4;
	if (ClientList[P].dMoney <= 100000.0)
		return VIP5;
	if (ClientList[P].dMoney <= 1000000.0)
		return VIP6;
	return VIP6;
}

void CClient::ApplyNewAccount()
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

	//分配卡号：
	int nSize = ClientList.size();
	pData->nCardNumb = ClientList[nSize - 1].nCardNumb + 1;
	pData->dMoney = 0.0;
	pData->dOverdraft = 0.0;
	pData->dOverdraftMoney = 0.0;
	pData->nVIP = VIP0;
	pData->nCardSecurity = Normal;
	pData->nPayForBank = 0.0;

	//储存新客户信息
	ClientList.push_back(*pData);
	Save();

	cout << endl << endl
		<< "欢迎 " << pData->sName << (pData->nSex == Male ? " 先生" : " 女士") << "成为招商银行客户!" << endl
		<< "您的银行卡登录用户名(卡号)为 " << pData->nCardNumb << endl;
	delete[]pData;

	system("pause");

	Save();
}
