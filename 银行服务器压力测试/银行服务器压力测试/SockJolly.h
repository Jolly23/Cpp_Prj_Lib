// SockJolly.h: interface for the CSockJolly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKJOLLY_H__C2C3E344_D2FE_477C_A327_7341E10C5AEA__INCLUDED_)
#define AFX_SOCKJOLLY_H__C2C3E344_D2FE_477C_A327_7341E10C5AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>

class SockJolly 
{
	SOCKET m_hSocket;

public:
	BOOL Create( UINT nSocketPort = 0, int nSocketType = SOCK_STREAM 
		, const char* lpszSocketAddress = NULL );

	int SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, LPCSTR lpszHostAddress = NULL, int nFlags = 0 );

	int ReceiveFrom( void* lpBuf, int nBufLen, char *rSocketAddress, UINT& rSocketPort, int nFlags = 0 );
	BOOL Listen( int nConnectionBacklog = 5 )
	{
		return !listen(m_hSocket,nConnectionBacklog);
	}
	int Send( const void* lpBuf, int nBufLen, int nFlags = 0 )
	{
		return send(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
	}
	int Receive( void* lpBuf, int nBufLen, int nFlags = 0 )
	{
		return recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
	}

	void Close( )
	{
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
};

#endif // !defined(AFX_SOCKJOLLY_H__C2C3E344_D2FE_477C_A327_7341E10C5AEA__INCLUDED_)
