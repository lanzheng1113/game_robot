#pragma once

#define _Harddisk_ID_MAXLEN		200

#include <ATLCOMTIME.H>

class TableUserInfo
{
public:
	TableUserInfo(void);
	~TableUserInfo(void);
public:
	DWORD		dwUser_ID;					// 1.������
	wchar_t		szUserPswd[32];				// 2.�������
	wchar_t		szCorrespondingGameID[32];	// 3.��Ӧ��Ϸ�˺�
	wchar_t		szUserSystemInfo[200];		// 4.�û�ϵͳ��Ӳ������Ϣ
	DWORD		dwCorrespondingGameType;	// 5.��Ӧ��Ϸ
	BOOL		bUseAble;					// 6.�Ƿ���ã��Ƿ񱻽��ã�
	DWORD		dwDllHash;					// 7.DLL Hash
	DWORD		dwAccSuccessCount;			// 8.�ɹ���֤����
	DWORD		dwUseTotalCount;			// 9.����֤����
	DWORD		dwAccType;					// 10.����˺�����	
	DWORD		vol;						// 11��� -- not use
	COleDateTime OleDtUserDeadLine;			// 12.ʹ������
	COleDateTime OleDtRegTime;				// 13.�û�ע��ʱ��
	COleDateTime OleDtReActiveTime;			// 14.�û����¿���ʱ�䣨����xx�����õ����ڣ�
	wchar_t		szProxyName[64];			// 15.��������
};
//DWORD	dwUserID;
//wchar_t	szPswd[_UserPSWMaxLen];
//DWORD	GameType;
//DWORD	KardType;
//BOOL	BisUsable;
//DWORD	vol;
//DWORD	dll_hash;
//char	HardDiskID[_Harddisk_ID_MAXLEN];
//wchar_t	szGameID[_GameIDMaxLEN]; 
//wchar_t	EnDeCryptKey[_RC5EnDeCryptKeyMaxLen+1];
//FILETIME	regtime;
//FILETIME	EndTime;
//FILETIME	ReActiveTime;
//DWORD	dwAccSuccessCount;
//DWORD	dwAccTotalCount; 
//DWORD	IP;