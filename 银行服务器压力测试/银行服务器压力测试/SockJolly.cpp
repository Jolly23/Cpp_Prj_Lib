// SockJolly.cpp: implementation of the CSockJolly class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SockJolly.h"
#pragma comment(lib,"ws2_32.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SockJolly::SockJolly()
{
	WSADATA wd;
	WSAStartup(0x0202,&wd);
	m_hSocket = INVALID_SOCKET;	//	INVALID_SOCKET = -1
}

SockJolly::~SockJolly()
{
	Close();
}

BOOL SockJolly::Create( UINT nSocketPort/* = 0 */, int nSocketType/* = SOCK_STREAM */
	, const char* lpszSocketAddress/* = NULL */ )
{
	m_hSocket = socket(AF_INET,nSocketType,0);

	if(m_hSocket == INVALID_SOCKET)
		return FALSE;

	return this->Bind(nSocketPort,lpszSocketAddress);
}

BOOL SockJolly::Bind( UINT nSocketPort, const char* lpszSocketAddress/* = NULL */)
{
	sockaddr_in sa = {AF_INET,htons(nSocketPort)};
	if(lpszSocketAddress)
		sa.sin_addr.S_un.S_addr = inet_addr(lpszSocketAddress);

	return !( bind(m_hSocket,(sockaddr*)&sa,sizeof(sa)) );
}

BOOL SockJolly::Accept( SockJolly& rConnectedSocket,  LPSTR szIP/* = NULL */,UINT *nPort/* = NULL */)
{
	sockaddr_in sa ;
	int nLen = sizeof(sa);
	rConnectedSocket.m_hSocket = accept(m_hSocket,(sockaddr*)&sa,&nLen);
	
	if(INVALID_SOCKET == rConnectedSocket.m_hSocket)
		return FALSE;

	if(szIP)
		strcpy(szIP,inet_ntoa(sa.sin_addr));

	if(nPort)
		*nPort = htons(sa.sin_port);

	return TRUE;
}

BOOL SockJolly::Connect(const char* lpszHostAddress, UINT nHostPort )
{
	sockaddr_in sa = {AF_INET,htons(nHostPort)};
	sa.sin_addr.S_un.S_addr = inet_addr(lpszHostAddress);

	return !connect(m_hSocket,(sockaddr*)&sa,sizeof(sa));
}

int SockJolly::ReceiveFrom( void* lpBuf, int nBufLen, char *rSocketAddress, UINT& rSocketPort, int nFlags/* = 0 */)
{
	return 0;
}

int SockJolly::SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, 
					  LPCSTR lpszHostAddress/* = NULL */, int nFlags/* = 0 */)
{
	sockaddr_in sa = {AF_INET,htons(nHostPort)};
	sa.sin_addr.S_un.S_addr = inet_addr(lpszHostAddress);

	return sendto(m_hSocket,(const char *)lpBuf,nBufLen,0,(sockaddr*)&sa,sizeof(sa));
}

BOOL SockJolly::GetSockName( char*  rSocketAddress, UINT& rSocketPort )
{
	sockaddr_in sa ;
	int nLen = sizeof(sa);

	if(getsockname(m_hSocket,(sockaddr*)&sa,&nLen))
		return FALSE;

	rSocketPort = htons(sa.sin_port);
	strcpy(rSocketAddress, inet_ntoa(sa.sin_addr));

	return TRUE;
}

BOOL SockJolly::GetPeerName(  char* rPeerAddress, UINT& rPeerPort )
{
	sockaddr_in sa ;
	int nLen = sizeof(sa);

	if(getpeername(m_hSocket,(sockaddr*)&sa,&nLen))
		return FALSE;

	rPeerPort = htons(sa.sin_port);
	strcpy(rPeerAddress, inet_ntoa(sa.sin_addr));

	return TRUE;
}
