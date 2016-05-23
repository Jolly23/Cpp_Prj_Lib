// Drcom密码提取_CSDN原版.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <basetsd.h>  
/************************************************************************/
/* Dr.com Ver 5.2.0 配置文件解密
/* By Douniwan5788@gmail.com
/************************************************************************/
void decrypt(UINT32 data1, UINT32 data2, UINT32 key1, UINT32 key2, UINT32 key3, UINT32 key4, PUINT32 value1, PUINT32 value2)
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

struct key_file {
	char filename[256];
	char key[32];
};

int  main(int   argc, char*   argv[])
{
	key_file kf[] = {
		{ "config","3J21d456j87jIOJ9" },          //账号密码  
		{ "DrConfigure","DrConfigure" },          //配置  
		{ "DrClient.gb","DrClient_Dict" },        //中文语言资源  
		{ "DrClient.en","DrClient_Dict" },        //英文语言资源  
		{ "DrLinkConfigure","DrConfigure" },  //who cares  
	};
	char path[256] = { "C:\\Drcom\\DrUpdateClient\\" };
	char strbuf[256];
	UINT32 keys[4], dec_data1, dec_data2, enc_data[3]; //extra 1 for scanf  

	for (key_file *pkf = kf; pkf - kf < sizeof(kf) / sizeof(key_file); pkf++) {
		for (int i = 0; i < 4; ++i)
		{
			memcpy(&keys[i], pkf->key + 4 * i, 4);
		}

		FILE *fpIn = fopen(pkf->filename, "r");
		if (!fpIn)
		{
			sprintf_s(strbuf, sizeof(strbuf), "%s%s", path, pkf->filename);
			fpIn = fopen(strbuf, "r");
			if (!fpIn)
			{
				printf("File not found, skip %s\n", pkf->filename);
				continue;
			}
		}

		sprintf_s(strbuf, sizeof(strbuf), "Decrypt_%s.txt", pkf->filename);
		FILE *fpOut = fopen(strbuf, "wb");
		if (!fpOut)
		{
			printf("Can't write %s! Skip!\n", strbuf);
			continue;
		}
		//fwrite("\xff\xfe",2,1,fpOut);  

		while (1) {
			for (int i = 0; i < 8; ++i)
			{
				if (fscanf_s(fpIn, "%02x", &((char *)enc_data)[i]) <= 0)
				{
					goto end;
				}
			}
			//printf("%08X %08X",enc_data[0],enc_data[1]);  
			decrypt(enc_data[0], enc_data[1], keys[0], keys[1], keys[2], keys[3], &dec_data1, &dec_data2);
			fwrite(&dec_data1, 4, 1, fpOut);
			fwrite(&dec_data2, 4, 1, fpOut);
		}
	end:
		fclose(fpIn);
		fclose(fpOut);
		printf("%s decrypt to %s finished\n", pkf->filename, strbuf);
	}

}