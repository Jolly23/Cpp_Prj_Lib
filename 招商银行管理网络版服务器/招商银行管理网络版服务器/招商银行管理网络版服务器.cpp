// 招商银行管理网络版服务器.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
CServers CServers::BankServers;

int main()
{
	/*已开始执行！*/
}























/*

BOOL CStudents::OnReceive(SockJolly *pSocka)
{
int nCmd = 0;
if(  (pSocka ->Receive(&nCmd,sizeof(int))) <= 0  )
return FALSE;
switch(nCmd)
{
case REQ_ADD:
cout << "请求添加数据" << endl;
return AddStud(pSocka);
break;
case REQ_DELALL:
cout << "请求删除全部数据" << endl;
break;
case REQ_DELONE:
cout << "请求删除单个数据" << endl;
return DeleteOne(pSocka);
case REQ_MOD:
cout << "请求修改单个数据" << endl;
return ModifyData(pSocka);
break;
case REQ_FINDBYNUMB:
cout << "请求查询单个数据" << endl;
break;
case REQ_BROW:
cout << "请求查看全部数据" << endl;
return SendAllData(pSocka);
case REQ_BROWONE:
cout << "请求查询单个数据" << endl;
return SendOneData(pSocka);
break;
case REQ_CNUMB:
cout << "请求检查学号是否存在" << endl;
return CheckOneNumb(pSocka);
break;
};
return TRUE;
}

*/