#pragma once
class CBankBigData
{
public:
	char sBigDataUpdataTimeBuff[64];

	double  dBankIncome;		//银行收入总数
	double  dBankSumMoney;		//银行客户存款总数
	UINT    nBankSumClient;		//银行客户总数

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

