#include "stdafx.h"
extern std::vector<ClientData> ClientList;
extern std::vector<AdminData> AdminList;

CBankBigData::CBankBigData() : dBankIncome(0.0), dBankSumMoney(0.0), nBankSumClient(0)
{
}


CBankBigData::~CBankBigData()
{
}

void CBankBigData::DataUpdata(const UINT unMins)
{
	GetTime(sBigDataUpdataTimeBuff);	//获取本次更新时间
	BaseDataUpdata();					//基础数据更新
	_ClientInterest(unMins);			//客户利息计算，每分钟1元
	_BankIncomeBySumMoney(unMins);		//银行用钱赚钱收入
	BaseDataUpdata();					//基础数据更新
	SaveData();							//保存数据
}

void CBankBigData::BaseDataUpdata()
{
	int nCountCL = ClientList.size(), i = 0;
	this->nBankSumClient = nCountCL;
	this->dBankIncome = 0.0;
	this->dBankSumMoney = 0.0;
	while (i < nCountCL)
	{
		this->dBankIncome += ClientList[i].dPayForBank;
		this->dBankSumMoney += ClientList[i].dMoney;
		++i;
	}
}

BOOL CBankBigData::GetBaseData(SockJolly *pSocka)
{
	int nRes = 1;
	char strBuff[64];
	pSocka->Send(&sBigDataUpdataTimeBuff, 64);
	std::cout << "当前数据更新时间：" << sBigDataUpdataTimeBuff << std::endl;

	pSocka->Send(&dBankIncome, sizeof(dBankIncome));
	sprintf(strBuff, "当前银行客户总数：%d 人", nBankSumClient);
	std::cout << strBuff << std::endl;

	pSocka->Send(&dBankSumMoney, sizeof(dBankSumMoney));
	sprintf(strBuff, "当前银行存款总数：%.2lf 元（RMB）", dBankSumMoney);
	std::cout << strBuff << std::endl;

	pSocka->Send(&nBankSumClient, sizeof(nBankSumClient));
	sprintf(strBuff, "当前银行总收入：%.2lf 元（RMB）", dBankIncome);
	std::cout << strBuff << std::endl;

	pSocka->Send(&nRes, sizeof(nRes));
	return TRUE;
}

void CBankBigData::_ClientInterest(const UINT unMins)
{
	int i = 0;
	while (i < nBankSumClient)
	{
		ClientList[i].dMoney += unMins;	//每分钟计息 1 元
		++i;
	}
}

void CBankBigData::_BankIncomeBySumMoney(const UINT unMins)
{
	//银行每分钟 可以赚到 总存款数额的0.1%
	double dEveryMoneyIncome = 0.0001 * unMins;
	int i = 0;
	while (i < nBankSumClient)
	{
		ClientList[i].dPayForBank += ClientList[i].dMoney * dEveryMoneyIncome;
		++i;
	}
}

void CBankBigData::SaveData()
{
	FILE* fpCL = fopen("./ClientData.lv", "wb");
	if (!fpCL)
		return;
	int j = 0;
	int nCountCL = ClientList.size();
	while (j < nCountCL)//内存->文件:依据循环到链表结尾
	{
		fwrite(&ClientList[j], 1, sizeof(ClientData), fpCL);//p ->data类型是struct SStud类型
		++j;
	}
	fclose(fpCL);
}

void CBankBigData::GetTime(char* rBuff)
{
	struct timeb tp;
	struct tm *tm;
	ftime(&tp);
	tm = localtime(&tp.time);
	sprintf(rBuff, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d",
		1900 + tm->tm_year,
		1 + tm->tm_mon,
		yday2mday(tm->tm_yday, 1900 + tm->tm_year),
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec,
		tp.millitm);
}

int CBankBigData::yday2mday(int nDays, int nYear)
{
	//天数自增1（1月1日为 0）
	nDays += 1;

	//判断平闰年，确定2月天数
	int nDaysOfFeb = 0;
	if (nYear % 4 == 0 && nYear % 100 != 0 || nYear % 400 == 0)
		nDaysOfFeb = 29;
	else
		nDaysOfFeb = 28;

	//返回年天数 - 月已过天数
	if (nDays <= 31)
		return nDays;
	else if (nDays <= 31 + nDaysOfFeb)
		return nDays - 31;
	else if (nDays <= 31 + nDaysOfFeb + 31)
		return nDays - 31 - nDaysOfFeb;
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30);
}