#pragma once

struct key_file {
	char filename[256];
	char key[32];
};

class CDecrypt
{
public:
	CDecrypt();
	~CDecrypt();

	key_file Info;
	void StartDecrypt(SockJolly *Sock);
	char& W_Char2Char(char w_char[], int LCLen);
	void  Decrypt();
	void  decrypt(UINT32 data1, UINT32 data2,
		UINT32 key1, UINT32 key2, UINT32 key3, UINT32 key4,
		PUINT32 value1, PUINT32 value2);
};

