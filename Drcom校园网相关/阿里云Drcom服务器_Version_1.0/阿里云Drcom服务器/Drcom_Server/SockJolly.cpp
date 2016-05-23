// SockJolly.cpp: implementation of the CSockJolly class.
//
//////////////////////////////////////////////////////////////////////

#include "SockJolly.h"
#pragma comment(lib,"ws2_32.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SockJolly::SockJolly()
{
#ifdef _WIN32
	WSADATA wd;
	WSAStartup(0x0202,&wd);
#endif
	m_hSocket = INVALID_SOCKET;	//	INVALID_SOCKET = -1
}

SockJolly::~SockJolly()	{
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
	socklen_t nLen = sizeof(sa);
	if(lpszSocketAddress)
		sa.sin_addr.s_addr = inet_addr(lpszSocketAddress);
	return !( bind(m_hSocket,(sockaddr*)&sa,nLen) );
}

BOOL SockJolly::Accept( SockJolly& rConnectedSocket,  LPSTR szIP/* = NULL */,UINT *nPort/* = NULL */)
{
	sockaddr_in sa ;
	socklen_t nLen = sizeof(sa);
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
	socklen_t nLen = sizeof(sa);
	sa.sin_addr.s_addr = inet_addr(lpszHostAddress);
	return !connect(m_hSocket,(sockaddr*)&sa,nLen);
}

int SockJolly::ReceiveFrom( void* lpBuf, int nBufLen, char *rSocketAddress, UINT& rSocketPort, int nFlags/* = 0 */)
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	int nRet = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, &nLen);
	if (nRet > 0) {
		rSocketPort = htons(sa.sin_port);
		strcpy(rSocketAddress, inet_ntoa(sa.sin_addr));
	}
	return nRet;
}

int SockJolly::SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, 
					  LPCSTR lpszHostAddress/* = NULL */, int nFlags/* = 0 */)
{
	sockaddr_in sa = {AF_INET,htons(nHostPort)};
	socklen_t nLen = sizeof(sa);
	sa.sin_addr.s_addr = inet_addr(lpszHostAddress);
	return sendto(m_hSocket,(const char *)lpBuf,nBufLen,0,(sockaddr*)&sa,nLen);
}

BOOL SockJolly::GetSockName( char*  rSocketAddress, UINT& rSocketPort )
{
	sockaddr_in sa ;
	socklen_t nLen = sizeof(sa);
	if(getsockname(m_hSocket,(sockaddr*)&sa,&nLen))
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	strcpy(rSocketAddress, inet_ntoa(sa.sin_addr));
	return TRUE;
}

BOOL SockJolly::GetPeerName(  char* rPeerAddress, UINT& rPeerPort )
{
	sockaddr_in sa ;
	socklen_t nLen = sizeof(sa);
	if(getpeername(m_hSocket,(sockaddr*)&sa,&nLen))
		return FALSE;
	rPeerPort = htons(sa.sin_port);
	strcpy(rPeerAddress, inet_ntoa(sa.sin_addr));
	return TRUE;
}


void SockJolly::GetTime(char* rBuff)
{
	struct timeb tp;
	struct tm *tm;
	ftime(&tp);
	tm = localtime(&tp.time);
	sprintf(rBuff, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d",
		1900 + tm->tm_year,
		1 + tm->tm_mon,
		yday2mday(tm->tm_yday, 1900 + tm->tm_year),
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec,
		tp.millitm);
}

int SockJolly::yday2mday(int nDays, int nYear)
{
	//天数自增1（1月1日为 0）
	nDays += 1;

	//判断平闰年，确定2月天数
	int nDaysOfFeb = 0;
	if (nYear % 4 == 0 && nYear % 100 != 0 || nYear % 400 == 0)
		nDaysOfFeb = 29;
	else
		nDaysOfFeb = 28;

	//返回年天数 - 月已过天数
	if (nDays <= 31)
		return nDays;
	else if (nDays <= 31 + nDaysOfFeb)
		return nDays - 31;
	else if (nDays <= 31 + nDaysOfFeb + 31)
		return nDays - 31 - nDaysOfFeb;
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31);
	else if (nDays <= 31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31)
		return nDays - (31 + nDaysOfFeb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30);
}