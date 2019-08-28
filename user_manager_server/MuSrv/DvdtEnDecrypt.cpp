#include "stdafx.h"
#include "DvdtEnDecrypt.h"
/*******************************************
������:
void	__stdcall	xorthis(unsigned char* lpEnDecryptDest,int XorEnDecryptDestlen,unsigned char* lpEnDecryptKey,const int iKeylen)
˵��:
ʹ��lpEnDecryptKey��Ϊ��Կ��lpEnDecryptDest���ܽ��ܡ�XorEnDecryptDestlenΪ���ģ����ģ�����.iKeylenΪ��Կ���ȡ�
��Կ�ĳ���Ϊ0x20��������ȡ��������0���㡣���ܺ������(�����ǽ��ܺ������)��Ȼ��������λ�ã����Ȳ��䡣
����ֵ��
��
������
unsigned char* lpEnDecryptDest����ţ��������ĺ�������ģ�/���������ĺ�������ģ��Ļ�����
int XorEnDecryptDestlen������/���ĳ���
unsigned char* lpEnDecryptKey����Կ
const int iKeylen����Կ����
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

const TCHAR p41str[] = "���н���������ǿ��Ϣ �����������Ժ������";

const int p41str_len = sizeof(p41str);
/*******************************************
������:
void	__stdcall	Xor2p41(unsigned char* lpEnDeCryptSrc,
int iSourcLen,
unsigned char* lpEnDeCryptDest)
˵��:
ʹ�ù̶���Կp41str�����Ľ���xor���ܽ��ܡ�
����ֵ��
��
������
unsigned char* lpEnDeCryptSrc������/����
int iSourcLen������/���ĳ���
unsigned char* lpEnDeCryptDest������/����
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