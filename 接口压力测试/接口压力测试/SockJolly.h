// SockJolly.h: interface for the CSockJolly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKJOLLY_H__C2C3E344_D2FE_477C_A327_7341E10C5AEA__INCLUDED_)
#define AFX_SOCKJOLLY_H__C2C3E344_D2FE_477C_A327_7341E10C5AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
using namespace std;


#ifdef _WIN32
#include <Winsock2.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")
typedef int socklen_t;
#else
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#define FALSE               0
#define TRUE                1

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL				*PBOOL;
typedef BOOL				*LPBOOL;
typedef BYTE				*PBYTE;
typedef BYTE				*LPBYTE;
typedef int					*PINT;
typedef int					*LPINT;
typedef WORD				*PWORD;
typedef WORD				*LPWORD;
typedef long				*LPLONG;
typedef DWORD				*PDWORD;
typedef DWORD				*LPDWORD;
typedef const char*			LPCTSTR;
typedef char*				LPTSTR;
typedef void				*LPVOID;
typedef char*				LPSTR;
typedef const char*			LPCSTR;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef unsigned int		SOCKET;

inline UINT WSAGetLastError() {
	return errno;
}
#define closesocket(Socket) close(Socket)
#endif // _WIN32


class SockJolly 
{
private:
	SOCKET m_hSocket;

public:
	BOOL Create( UINT nSocketPort = 0, int nSocketType = SOCK_STREAM,const char* lpszSocketAddress = NULL );

	int SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, LPCSTR lpszHostAddress = NULL, int nFlags = 0 );

	int ReceiveFrom( void* lpBuf, int nBufLen, char *rSocketAddress, UINT& rSocketPort, int nFlags = 0 );
	
	BOOL Listen(int nConnectionBacklog = 5) {
		return !listen(m_hSocket, nConnectionBacklog);
	}

	int Send(const void* lpBuf, int nBufLen, int nFlags = 0) {
		return send(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
	}

	int Receive(void* lpBuf, int nBufLen, int nFlags = 0) {
		return recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
	}

	void Close() {
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}

	BOOL GetSockName( char*  rSocketAddress, UINT& rSocketPort );
	BOOL GetPeerName(  char* rPeerAddress, UINT& rPeerPort );
	BOOL Accept( SockJolly& rConnectedSocket,  LPSTR szIP = NULL,UINT *nPort = NULL);
	BOOL Connect(const char* lpszHostAddress, UINT nHostPort );
	BOOL Bind( UINT nSocketPort, const char* lpszSocketAddress = NULL );

	SockJolly();
	virtual ~SockJolly();

	//附加功能：获取时间
	void GetTime(char* rBuff);
	int yday2mday(int nDays, int nYear);
};

#endif // !defined(AFX_SOCKJOLLY_H__C2C3E344_D2FE_477C_A327_7341E10C5AEA__INCLUDED_)
