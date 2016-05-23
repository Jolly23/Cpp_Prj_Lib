// BankAdmin.cpp: implementation of the CBankAdmin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BankAdmin.h"
using namespace std;
vector<SAdmin> AdminList;
extern vector<ClientData> ClientList;
enum Prior { P0 = 935007, P1 };
enum Sex { Male = 10086, Female };
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBankAdmin::CBankAdmin()
{
	this->Load();

	if (CheckLoginName("adminp0") == -1) {
		SAdmin *pDefault = new SAdmin;
		strcpy(pDefault->sName, "CMBC����");
		pDefault->nSex = Male;
		strcpy(pDefault->sLoginName, "adminp0");
		strcpy(pDefault->sPass, "123456");
		pDefault->lfSalary = 0.0;
		pDefault->unPrior = P0;
		AdminList.push_back(*pDefault);
		delete[]pDefault;
	}

	if (CheckLoginName("adminp1") == -1) {
		SAdmin *pDefault = new SAdmin;
		strcpy(pDefault->sName, "CMBC����");
		pDefault->nSex = Male;
		strcpy(pDefault->sLoginName, "adminp1");
		strcpy(pDefault->sPass, "123456");
		pDefault->lfSalary = 0.0;
		pDefault->unPrior = P1;
		AdminList.push_back(*pDefault);
		delete[]pDefault;
	}

	if (AdminList[CheckLoginName("adminp0")].unPrior != P0)
		AdminList[CheckLoginName("adminp0")].unPrior = P0;

	if (AdminList[CheckLoginName("adminp1")].unPrior != P1)
		AdminList[CheckLoginName("adminp1")].unPrior = P1;

	this->LoginRoot();
}

CBankAdmin::~CBankAdmin()
{
	this->Save();
}

void CBankAdmin::LoginRoot()
{
	int nResult = this->Login();
	if (nResult == -1)
		return;
	Menu(nResult);
}

int CBankAdmin::Login()
{
	system("cls");

	/***************************************/
	/***************************************/
	///////////////////////////////////
	int iii = 0;
	int nnnCount = AdminList.size();
	while (iii < nnnCount)
	{
		cout << iii + 1 << "��" << AdminList[iii].sLoginName << "\t" << AdminList[iii].sName
			<< "\t" << AdminList[iii].sPass << endl;
		++iii;
	}
	///////////////////////////////////
	/***************************************/
	/***************************************/


	int nCount = AdminList.size();
	char sLName[32];
	cout << "�������¼�û�����";
	cin >> sLName;

	int nResult = CheckLoginName(sLName);

	if (nResult == -1) {
		cout << endl << "���û��������ڣ�" << endl;
		system("pause");
		return -1;
	}

	char sLPass[32];
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
		sLPass[i++] = ch;
	}
	sLPass[i] = '\0';

	if (strcmp(sLPass, AdminList[nResult].sPass) == 0)
		return nResult;
	cout << endl << "�������" << endl;
	system("pause");
	return -1;
}

int CBankAdmin::CheckLoginName(char sLName[])
{
	int i = 0;
	int nCount = AdminList.size();
	while (i < nCount)
	{
		if (stricmp(sLName, AdminList[i].sLoginName) == 0)
			return i;
		++i;
	}
	return -1;
}

void CBankAdmin::Load()
{
	AdminList.clear();
	FILE* fp = fopen("./CMBCAdmin.lv", "rb");
	if (!fp)
		return;
	SAdmin data;
	while (fread(&data, 1, sizeof(SAdmin), fp)>0)
		AdminList.push_back(data);
	fclose(fp);
}

void CBankAdmin::Save()
{
	FILE* fp = fopen("./CMBCAdmin.lv", "wb");
	if (!fp)
	{
		cout << "�����ļ�ʱʧ�ܣ�" << endl;
		return;
	}
	int i = 0;
	int nCount = AdminList.size();
	while (i < nCount)//�ڴ�->�ļ�:����ѭ���������β
	{
		fwrite(&AdminList[i], 1, sizeof(SAdmin), fp);//p ->data������struct SStud����	
		++i;
	}
	fclose(fp);
}


void CBankAdmin::Menu(int DAdmin)
{
	system("cls");
	SAdmin &Admin = AdminList[DAdmin];
	cout << Admin.sName << (Admin.nSex == Male ? " ����" : " Ůʿ") << "����ӭ��½�������й���ϵͳ��" << endl;
	cout << endl;

	cout << "1.�������а칫ϵͳ" << endl
		<< "2.�޸ĸ����˺���Ϣ" << endl;
	if (Admin.unPrior == P1)
		cout << "3.�������й���Ա��Ϣ" << endl;

	system("pause");
}
