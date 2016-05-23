// BankData.cpp: implementation of the CBankData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BankData.h"
using namespace std;

extern vector<SAdmin> AdminList;
extern vector<ClientData> ClientList;
enum CardSecurity { Normal = 486, Freeze, Loss };
enum VIP { VIP0 = 4894613, VIP1, VIP2, VIP3, VIP4, VIP5, VIP6, Abnormal };
enum Sex { Male = 10086, Female };
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBankData::CBankData()
{

}

CBankData::~CBankData()
{

}
