#pragma once

class UserInfo
{
public:
	UserInfo::UserInfo()
		:_isBind(FALSE),
		_deadLine(0),
		_createTime(0),
		_KardType(0)
	{
		memset(_accInfo,0,sizeof(_accInfo));
		memset(_nameOfOwner,0,sizeof(_nameOfOwner));
	}

	char _accInfo[10];
	char _nameOfOwner[24];
	DWORD _KardType;
	BOOL _isBind;
	time_t _deadLine;
	time_t _createTime;
protected:
private:
};


