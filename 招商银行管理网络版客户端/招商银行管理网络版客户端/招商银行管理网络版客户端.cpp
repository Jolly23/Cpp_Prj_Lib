// �������й��������ͻ���.cpp : �������̨Ӧ�ó������ڵ㡣
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
	/*****************************�������ʱ������������*******************************/
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
		cout << "�������ʧ�ܣ��������Ӵ��� Error = " << nError << endl;
		system("pause");
		return FALSE;
	}
	catch (...) {
		cout << "����δ֪����" << endl;
		return FALSE;
	}
}

void SWClose()
{
	/***********************************����ر�************************************/
	SockJolly sockc;
	sockc.Create();
	sockc.Connect(ServerIP, ServerPort);
	int nCloseSignal = 1112;
	sockc.Send(&nCloseSignal, sizeof(nCloseSignal));
	sockc.Close();
}