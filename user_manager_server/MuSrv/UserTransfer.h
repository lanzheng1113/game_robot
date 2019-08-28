#pragma once
#include "util/Singleton.h"
#include <vector>
#include "../common/UserInforDef.h"

class CMyNineXUsr;

class CUserTransfer : public Singleton<CUserTransfer>
{
public:
	CUserTransfer(void);
	~CUserTransfer(void);
	//将CMyNineXUsr转换为UserInfo用于传送给客户端。
	BOOL FromNineXUserVec(const std::vector<CMyNineXUsr*>& vec);

	UserInfo* GetUserInfo() const { return _userInfoBuffer; }
	DWORD GetCountOfUserInfo() const { return _countOfUserInfo; }
private:
	UserInfo* _userInfoBuffer; //装用户信息的缓冲区。
	DWORD _countOfUserInfo;//缓冲区中UserInfo的个数。
};
