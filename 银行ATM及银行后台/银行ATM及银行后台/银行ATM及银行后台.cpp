// ����ATM�����к�̨.cpp : �������̨Ӧ�ó������ڵ㡣
//


/*
�������
���У�
�������û���¼�������ڲ��������ݼ��������ܣ�
1. ������ѡ����� ���к�̨ or ATM��
2. ֻ�����й���Ա�˻�ȷ�Ϻ���ܽ��к�̨����
3. ���й���Ա�˻��зּ�����Ϊ��֧�С����С����С����У�
{
(P1)���У��������޸����� + �޸� �˻����������֤�š���ʧ�˻�
(P0)���У�*����Ȩ�� + ֱ���޸��˻���� + �������ݷ��� + �����˻� + �鿴&�޸Ĺ���Ա�˻���Ϣ
}
*ÿ������Ա�������޸��Լ����˻�����(�����޸Ĺ���Ա������Ϣ)

�û���
ATM����ȡת�˲���;
1. ƾ�û���+���� ��¼ATM��
2. ������
3. ȡ�����(��ͨ������͸֧�����ÿ���͸֧)
4. ���˺Ž���ֱ��ת��(ȷ��ʱֻ��ʾ ��+�ƺ�)
5. �޸�ȡ������
*/


#include "stdafx.h"
#include "BankAdmin.h"
#include "ATM.h"
#include <conio.h>
#include <time.h>
#include <iostream>
using std::cout;
using std::endl;
void Menu();

void main()
{
	do {
		system("cls");
		Menu();
	} while (true);
}

void Menu()
{
	cout << "��ѡ���¼����" << endl
		<< "1.���й���ϵͳ" << endl
		<< "2.ATM ����ϵͳ" << endl
		<< "3.  ��������" << endl
		<< "0.��ȫ�˳�ϵͳ" << endl;
	char cSel = getch();

	if (cSel == '1') {
		CBankAdmin Admin;
		return;
	}

	if (cSel == '2') {
		CATM atm;
		return;
	}

	if (cSel == '3') {
		CClient New;
		New.Load();
		New.ApplyNewAccount();
		return;
	};

	if (cSel == '0')
		return;
}
