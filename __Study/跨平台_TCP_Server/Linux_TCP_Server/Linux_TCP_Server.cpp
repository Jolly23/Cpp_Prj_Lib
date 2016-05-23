#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
using namespace std;

#ifdef _WIN32
	#include <Winsock2.h>
	#pragma comment(lib,"ws2_32.lib")
	typedef int socklen_t;
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <errno.h>
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
	typedef unsigned int SOCKET;
#endif // _WIN32


const int ServerPort = 50151;
int main()
{
#ifdef _WIN32
	WSADATA wd;
	WSAStartup(0x0202, &wd);
#endif // _WIN32

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sa = { AF_INET,htons(ServerPort) };
	int n = bind(sock, (sockaddr*)&sa, sizeof(sa));
	listen(sock, 5);
	socklen_t nLen = sizeof(sa);
	char sMessage[256] = { 0 };

	cout << "Start Service!" << endl;
	while (true)
	{
		SOCKET socka = accept(sock, (sockaddr*)&sa, &nLen);
		cout << inet_ntoa(sa.sin_addr) << "-" << htons(sa.sin_port) << " Login" << endl;
		while (true)
		{
			n = recv(socka, sMessage, sizeof(sMessage), 0);
			if (n <= 0)
				break;
			sMessage[n] = '\0';
			cout << inet_ntoa(sa.sin_addr) << ": " << sMessage << endl;
		}
		cout << inet_ntoa(sa.sin_addr) << "-" << htons(sa.sin_port) << " Logout" << endl;
	}
	return 0;
}