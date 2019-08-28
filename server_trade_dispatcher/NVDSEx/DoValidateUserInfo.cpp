#include "stdafx.h"
#include "DoValidateUserInfo.h"

TableUserInfo::TableUserInfo( void )
:OleDtUserDeadLine(1900,1,1,0,0,0)
,OleDtReActiveTime(1900,1,1,0,0,0)
,OleDtRegTime(2019,1,1,0,0,0)
{
	dwUser_ID = 0;
	dwCorrespondingGameType = 0;
	bUseAble = FALSE;				// 6.是否可用（是否被禁用）
	dwDllHash = 0;					// 7.DLL Hash
	dwAccSuccessCount = 0;			// 8.成功验证次数
	dwUseTotalCount = 0;			// 9.总验证次数
	dwAccType = 0;					// 10.外挂账号类型	
	vol = 0;						// 11卷标 -- not use
	memset(szUserPswd,0,sizeof(szUserPswd));
	memset(szUserSystemInfo,0,sizeof(szUserSystemInfo));
	memset(szCorrespondingGameID,0,sizeof(szCorrespondingGameID));
	memset(szProxyName,0,sizeof(szProxyName));
}

TableUserInfo::~TableUserInfo( void )
{

}