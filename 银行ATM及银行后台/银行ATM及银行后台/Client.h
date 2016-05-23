#pragma once
#if !defined(AFX_CLIENT_H__F2EED9D4_9C81_4AA5_B01C_D4716248FAAD__INCLUDED_)
#define AFX_CLIENT_H__F2EED9D4_9C81_4AA5_B01C_D4716248FAAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ClientData
{
	char sName[20];		//�ֿ�������
	int  nSex;			//�ֿ����Ա�
	int  nCardNumb;		//����(��Ϊ��¼��)
	char sPass[7];		//��λ����
	double dMoney;		//���
	int  nVIP;			//�ͻ��ȼ�(VIP0 ~ VIP6)
	double dOverdraft;	//͸֧���( = 0 ��Ϊ����û����� 0 ��Ϊ���ÿ��û�)
	double dOverdraftMoney; //��͸֧���
	int  nCardSecurity;	//��Ƭ��ȫ״̬(��ʧ������)
	double nPayForBank;	//�����д���������
};

class CClient
{
public:
	void ApplyNewAccount();
	int isVIP(int CNumb);
	void Load();
	void Save();
	int  Login();
	int  CheckCardNumb(int Numb);
	CClient();
	~CClient();
};

#endif // !defined(AFX_CLIENT_H__F2EED9D4_9C81_4AA5_B01C_D4716248FAAD__INCLUDED_)
