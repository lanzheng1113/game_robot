#include "stdafx.h"
#include "DvdtEnDecrypt.h"
/*******************************************
函数名:
void	__stdcall	xorthis(unsigned char* lpEnDecryptDest,int XorEnDecryptDestlen,unsigned char* lpEnDecryptKey,const int iKeylen)
说明:
使用lpEnDecryptKey作为密钥对lpEnDecryptDest加密解密。XorEnDecryptDestlen为明文（密文）长度.iKeylen为密钥长度。
密钥的长度为0x20，过长截取，不足用0补足。加密后的密文(或者是解密后的明文)依然放在明文位置，长度不变。
返回值：
无
参数：
unsigned char* lpEnDecryptDest：存放（输入明文和输出密文）/（输入密文和输出明文）的缓冲区
int XorEnDecryptDestlen：明文/密文长度
unsigned char* lpEnDecryptKey：密钥
const int iKeylen：密钥长度
*******************************************/
void	__stdcall	xorthis(unsigned char* lpEnDecryptDest,
							int XorEnDecryptDestlen,
							unsigned char* lpEnDecryptKey,
							const int iKeylen)
{
	unsigned char RealKey[0x20] = {0};
	memset(RealKey,0,0x20);
	int RealKeyLen = 0x20;
	int iActualCopyLen = 0;
	if (iKeylen < 0x20)
	{
		iActualCopyLen = iKeylen;
	}
	else
	{
		iActualCopyLen = 0x20;
	}
	memcpy(RealKey,lpEnDecryptKey,iActualCopyLen);

	int i = 0;
	while(XorEnDecryptDestlen)
	{
		*lpEnDecryptDest = *lpEnDecryptDest ^ RealKey[i];
		i++;
		if(i>=RealKeyLen)
			i=0;
		XorEnDecryptDestlen--;
		lpEnDecryptDest++;
	}
}

const TCHAR p41str[] = "天行健君子以自强不息 地势坤君子以厚德载物";

const int p41str_len = sizeof(p41str);
/*******************************************
函数名:
void	__stdcall	Xor2p41(unsigned char* lpEnDeCryptSrc,
int iSourcLen,
unsigned char* lpEnDeCryptDest)
说明:
使用固定密钥p41str对明文进行xor加密解密。
返回值：
无
参数：
unsigned char* lpEnDeCryptSrc：明文/密文
int iSourcLen：明文/密文长度
unsigned char* lpEnDeCryptDest：密文/明文
*******************************************/
void	__stdcall	Xor2p41(unsigned char* lpEnDeCryptSrc,
							int iSourcLen,
							unsigned char* lpEnDeCryptDest)
{
	int i = (*lpEnDeCryptSrc)%p41str_len;
	lpEnDeCryptSrc++;
	while(--iSourcLen)
	{
		*lpEnDeCryptDest = *lpEnDeCryptSrc ^ *((unsigned char*)p41str + i);
		i++;
		if(i>=p41str_len)i=0;
		lpEnDeCryptSrc++;	lpEnDeCryptDest++;
	}
}

__declspec(naked) unsigned long myhash(unsigned char* lpstr,int len)
{
	__asm
	{
		push ebp
			mov ebp,esp
			push	esi
			push	edx
			push	ecx
			mov	esi,lpstr
			xor	edx,edx
			xor	eax,eax
			mov	ecx,len
myhash_loop:
		lodsb
			ror edx,3
			add edx,eax
			loop	myhash_loop
			mov	eax,edx
			pop	ecx
			pop	edx
			pop	esi
			mov esp,ebp
			pop ebp
			retn
	};
}