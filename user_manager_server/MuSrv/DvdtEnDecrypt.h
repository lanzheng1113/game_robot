#ifndef _DVDTDEF_H
#define _DVDTDEF_H

//计算HASH的函数
unsigned long myhash(unsigned char* lpstr,int len);

//对一个字符串异或的函数
void	__stdcall	Xor2p41(unsigned char* lpEnDeCryptSrc,
							int iSourcLen,
							unsigned char* lpEnDeCryptDest);

//对以个字符串异或的函数
void	__stdcall	xorthis(unsigned char* lpEnDecryptDest,
							int XorEnDecryptDestlen,
							unsigned char* lpEnDecryptKey,
							const int iKeylen);


#endif
