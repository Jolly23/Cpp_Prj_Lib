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
	GetTime(sBigDataUpdataTimeBuff);	//��ȡ���θ���ʱ��
	BaseDataUpdata();					//�������ݸ���
	_ClientInterest(unMins);			//�ͻ���Ϣ���㣬ÿ����1Ԫ
	_BankIncomeBySumMoney(unMins);		//������Ǯ׬Ǯ����
	BaseDataUpdata();					//�������ݸ���
	SaveData();							//��������
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
	std::cout << "��ǰ���ݸ���ʱ�䣺" << sBigDataUpdataTimeBuff << std::endl;

	pSocka->Send(&dBankIncome, sizeof(dBankIncome));
	sprintf(strBuff, "��ǰ���пͻ�������%d ��", nBankSumClient);
	std::cout << strBuff << std::endl;

	pSocka->Send(&dBankSumMoney, sizeof(dBankSumMoney));
	sprintf(strBuff, "��ǰ���д��������%.2lf Ԫ��RMB��", dBankSumMoney);
	std::cout << strBuff << std::endl;

	pSocka->Send(&nBankSumClient, sizeof(nBankSumClient));
	sprintf(strBuff, "��ǰ���������룺%.2lf Ԫ��RMB��", dBankIncome);
	std::cout << strBuff << std::endl;

	pSocka->Send(&nRes, sizeof(nRes));
	return TRUE;
}

void CBankBigData::_ClientInterest(const UINT unMins)
{
	int i = 0;
	while (i < nBankSumClient)
	{
		ClientList[i].dMoney += unMins;	//ÿ���Ӽ�Ϣ 1 Ԫ
		++i;
	}
}

void CBankBigData::_BankIncomeBySumMoney(const UINT unMins)
{
	//����ÿ���� ����׬�� �ܴ�������0.1%
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
	while (j < nCountCL)//�ڴ�->�ļ�:����ѭ���������β
	{
		fwrite(&ClientList[j], 1, sizeof(ClientData), fpCL);//p ->data������struct SStud����
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
	//��������1��1��1��Ϊ 0��
	nDays += 1;

	//�ж�ƽ���꣬ȷ��2������
	int nDaysOfFeb = 0;
	if (nYear % 4 == 0 && nYear % 100 != 0 || nYear % 400 == 0)
		nDaysOfFeb = 29;
	else
		nDaysOfFeb = 28;

	//���������� - ���ѹ�����
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