#pragma once
#include "SockJolly.h"

/*****************��������ַ���˿�*******************/
const char ServerIP[20] = "120.27.126.195";
const int  ServerPort = 40151;
////////////////////////////////////////////////////

class SockJollyEx :
	public SockJolly
{
public:
	SockJollyEx() 
	{
		this->Create();
		this->Connect(ServerIP, ServerPort);
	}
	~SockJollyEx() 
	{
	}
};

