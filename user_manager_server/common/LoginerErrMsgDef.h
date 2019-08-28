#pragma once

#define ErrorString_CanNot_GetMacs L"CannotGetMacs"
enum enum_LoginerErrorCode{
	ELE_OK = 1000,
	ELE_User_NotUsable = 400,
	ELE_WrongMacs = 402,
	ELE_WrongIdWithPrevious = 403,
	ELE_UserOrPasswordError = 404,
	ELE_UnknowTypeId = 405,
	ELE_UserTimeOut = 406,
	ELE_UserAlreadyOnline = 407,
};