// Drcom������ſͻ���.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Decrypt.h"
using namespace std;

//TCP_Server_Addr
const char ServerIP[20]	= "120.27.126.195";
const UINT ServerPort	= 10150;

void main()
{
	SockJolly sock;
	sock.Create();
	cout << "��ʼ���ӷ�����..." << endl;
	sock.Connect(ServerIP, ServerPort);
	if (0 != WSAGetLastError()) {
		cout << "���ӷ�����ʧ�ܣ��������=" << WSAGetLastError() << endl;
		cin.get();
		return;
	}
	system("cls");
	cout << "���������ӳɹ���" << endl;

	//��ȡ����
	CDecrypt DeMaster;
	DeMaster.StartDecrypt(&sock);

	char sSend[256];
	while (true)
	{
		fflush(stdin);
		cout << "�����뷢�����ݣ�";
		cin >> sSend;
		sock.Send(sSend, strlen(sSend));
	}
	sock.Close();
	cout << "�ѶϿ����ӣ�" << endl;
	cin.get();
}
