#pragma once
class CBankBigData
{
public:
	char sBigDataUpdataTimeBuff[64];

	double  dBankIncome;		//������������
	double  dBankSumMoney;		//���пͻ��������
	UINT    nBankSumClient;		//���пͻ�����

	CBankBigData();
	~CBankBigData();
	BOOL GetBaseData(SockJolly *pSocka);
	void _ClientInterest(const UINT unMins);
	void _BankIncomeBySumMoney(const UINT unMins);
	void BaseDataUpdata();
	void GetTime(char* rBuff);
	int  yday2mday(int nDays, int nYear);
	void DataUpdata(const UINT unMins);
	void SaveData();
};

