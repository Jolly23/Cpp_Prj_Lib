#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
#ifndef _WIN32
	inline unsigned int WSAGetLastError() {
		return errno;
	}
#endif

const int ServerPort = 50151;
//TCP_Client
int main()
{
#ifdef _WIN32
	WSADATA wd;
	WSAStartup(0x0202, &wd);
#endif // _WIN32

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Socket Error:" << WSAGetLastError() << endl;
		return -1;
	}
	sockaddr_in sa = { AF_INET,htons(ServerPort) };			//服务器PORT
	sa.sin_addr.s_addr = inet_addr("120.27.126.195");	//服务器IP
	socklen_t nLen = sizeof(sa);

	int nRes = connect(sock, (sockaddr*)&sa, nLen);
	if (nRes < 0) {
		cout << "Connect Error:" << WSAGetLastError() << endl;
		cin.get();
		return -1;
	}

	cout << "客户端：" << endl;
	char sMessage[256];
	while (true)
	{
		cout << "请输入你要发送的内容[#退出]：";
		cin >> sMessage;
		if (sMessage[0] == '#')
			break;
		send(sock, sMessage, strlen(sMessage), 0);
	}
	return 0;
}

