#ifndef _DVDTDEF_H
#define _DVDTDEF_H

//����HASH�ĺ���
unsigned long myhash(unsigned char* lpstr,int len);

//��һ���ַ������ĺ���
void	__stdcall	Xor2p41(unsigned char* lpEnDeCryptSrc,
							int iSourcLen,
							unsigned char* lpEnDeCryptDest);

//���Ը��ַ������ĺ���
void	__stdcall	xorthis(unsigned char* lpEnDecryptDest,
							int XorEnDecryptDestlen,
							unsigned char* lpEnDecryptKey,
							const int iKeylen);


#endif
