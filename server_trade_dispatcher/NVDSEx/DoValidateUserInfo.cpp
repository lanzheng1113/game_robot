#include "stdafx.h"
#include "DoValidateUserInfo.h"

TableUserInfo::TableUserInfo( void )
:OleDtUserDeadLine(1900,1,1,0,0,0)
,OleDtReActiveTime(1900,1,1,0,0,0)
,OleDtRegTime(2019,1,1,0,0,0)
{
	dwUser_ID = 0;
	dwCorrespondingGameType = 0;
	bUseAble = FALSE;				// 6.�Ƿ���ã��Ƿ񱻽��ã�
	dwDllHash = 0;					// 7.DLL Hash
	dwAccSuccessCount = 0;			// 8.�ɹ���֤����
	dwUseTotalCount = 0;			// 9.����֤����
	dwAccType = 0;					// 10.����˺�����	
	vol = 0;						// 11��� -- not use
	memset(szUserPswd,0,sizeof(szUserPswd));
	memset(szUserSystemInfo,0,sizeof(szUserSystemInfo));
	memset(szCorrespondingGameID,0,sizeof(szCorrespondingGameID));
	memset(szProxyName,0,sizeof(szProxyName));
}

TableUserInfo::~TableUserInfo( void )
{

}