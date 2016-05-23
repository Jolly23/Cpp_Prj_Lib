#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "SockJolly.h"
#include <string.h>
#include <iostream>
using namespace std;


const int ServerPort = 50151;
int main()
{
	SockJolly sock;
	if (!sock.Create(ServerPort)) {
		cout << "Create PORT ERROR=" << WSAGetLastError() << endl;
		return 0;
	}
	sock.Listen();
	char sIP[20];
	UINT nPort;
	char sMessage[512];
	SockJolly *pSocka = new SockJolly;
	int n = 0;
	while (sock.Accept(*pSocka,sIP,&nPort)) {
		cout << sIP << "-" << nPort << " -Login" << endl;
		while (TRUE) {
			n = pSocka->Receive(sMessage, sizeof(sMessage));
			if (n <= 0)
				break;
			sMessage[n] = '\0';
			cout << sIP << ": " << sMessage << endl;
		}
		cout << sIP << "-" << nPort << " -Logout" << endl;
	}
	delete pSocka;
	return 0;
}
