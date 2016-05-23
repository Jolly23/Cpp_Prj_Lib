// 招商银行管理网络版客户端.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;

BOOL StartTest();
void SWClose();

void main()
{
	if (!StartTest()) {
		return;
	}

	CHallo NewPage;
	while (NewPage.HalloMenu())	
		;

	SWClose();
}

BOOL StartTest()
{
	/*****************************软件启动时测试网络连接*******************************/
	try
	{
		SockJolly sock;
		sock.Create();
		sock.Connect(ServerIP, ServerPort);
		if (0 != WSAGetLastError()) {
			throw WSAGetLastError();
		}
		int nStartSignal = 1111;
		sock.Send(&nStartSignal, sizeof(nStartSignal));
		sock.Close();
		return TRUE;
	}
	catch (int nError)
	{
		cout << "软件启动失败，网络连接错误！ Error = " << nError << endl;
		system("pause");
		return FALSE;
	}
	catch (...) {
		cout << "发生未知错误！" << endl;
		return FALSE;
	}
}

void SWClose()
{
	/***********************************软件关闭************************************/
	SockJolly sockc;
	sockc.Create();
	sockc.Connect(ServerIP, ServerPort);
	int nCloseSignal = 1112;
	sockc.Send(&nCloseSignal, sizeof(nCloseSignal));
	sockc.Close();
}