#pragma once
#include "SockJolly.h"

struct ClientData
{
	char sName[20];			//�ֿ�������
	int  nSex;				//�ֿ����Ա�
	int  nCardNumb;			//����(��Ϊ��¼��)
	char sPass[7];			//��λ����
	double dMoney;			//���
	int  nVIP;				//�ͻ��ȼ�(VIP0 ~ VIP6)
	double dOverdraft;		//͸֧���( = 0 ��Ϊ����û����� 0 ��Ϊ���ÿ��û�)
	double dOverdraftMoney; //��͸֧���
	int  nCardSecurity;		//��Ƭ��ȫ״̬(��ʧ������)
	double dPayForBank;		//�����д���������
};

struct ClientLoginData
{
	int  nCardNumb;
	char sPassword[7];
};

class CClient :
	public SockJolly
{
public:
	CClient();
	~CClient();
	void LoadClientData();
	void SaveClientData();

	BOOL ClientLogin(SockJolly *pSocka);
	BOOL isCardNumbExist(SockJolly *pSocka);
	BOOL ApplyNewCardNumb(SockJolly *pSocka);
	BOOL CreateNewAccount(SockJolly *pSocka);
	BOOL AppleCardSecuritySituation(SockJolly *pSocka);
	BOOL ApplyDepositMoney(SockJolly *pSocka);
	BOOL CheckSumMoney(SockJolly *pSocka);
	BOOL GetVipLevel(SockJolly *pSocka);
	BOOL DrawMoneyAndPoundage(SockJolly *pSocka);
	BOOL TransferMoney(SockJolly *pSocka);
	BOOL ApplyCardBasicData(SockJolly *pSocka);
	BOOL ApplyGetCardPassword(SockJolly *pSocka);
	BOOL ApplyFreezeCard(SockJolly *pSocka);
	BOOL ApplyChangePassword(SockJolly *pSocka);
	int VipLevelCalc(int nCardNumb, double dSumMoney);
};