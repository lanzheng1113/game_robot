#include "StdAfx.h"
#include "UserTransfer.h"
#include "../common/MyNineXUsr.h"
#include "util/Logger.h"
#include "util/DateTime.h"
#include "strsafe.h"
#include "DebugMessage.h"

CUserTransfer::CUserTransfer(void)
:_userInfoBuffer(NULL),
_countOfUserInfo(0)
{
}

CUserTransfer::~CUserTransfer(void)
{
}

typedef std::vector<CMyNineXUsr*>::const_iterator My9XVecIter;

BOOL CUserTransfer::FromNineXUserVec( const std::vector<CMyNineXUsr*>& vec )
{
	if (vec.empty()){
		return FALSE;
	}

	if (_userInfoBuffer)
	{
		delete [] _userInfoBuffer;
		_userInfoBuffer = NULL;
		_countOfUserInfo = 0;
	}

	_userInfoBuffer = new UserInfo[vec.size()];
	_countOfUserInfo = vec.size();

	int indexOfCC = 0;
	for (My9XVecIter it = vec.begin(); it != vec.end(); ++it){
		CMyNineXUsr* mnx = *it;
		if (mnx == NULL){
			LOG_ERROR("严重的BUG，在转换的时候发现了一个空的。");
			return FALSE;
		}
		StringCbCopy(_userInfoBuffer[indexOfCC]._accInfo,sizeof(_userInfoBuffer[indexOfCC]._accInfo),mnx->getAuthId().c_str());//1
		StringCbCopy(_userInfoBuffer[indexOfCC]._nameOfOwner,sizeof(_userInfoBuffer[indexOfCC]._nameOfOwner),mnx->getName().c_str());
		std::string sdl = mnx->getDate_of_end();
		//DebugMessage("send time = %s",sdl.c_str());
		if (sdl == unbinded_tag){
			sdl = "1988-08-08 08:08:08";
		}
		DateTime dt;
		bool bxx = DateTime::fromString(sdl,dt);
		if (!bxx){
			dt = DateTime();
		}
		_userInfoBuffer[indexOfCC]._deadLine = dt.toTime_tFormat(); //2
		
		_userInfoBuffer[indexOfCC]._KardType = atoi(mnx->getCardDaysType().c_str());

		std::string sdc = mnx->getUserCreateTime();
		DateTime::fromString(sdc,dt);
		_userInfoBuffer[indexOfCC]._createTime = dt.toTime_tFormat(); //3

		_userInfoBuffer[indexOfCC]._isBind = !(mnx->getHarddisk() == "未绑定" || mnx->getHarddisk() == ""); //4

		indexOfCC ++;
	}

	return TRUE;
}

