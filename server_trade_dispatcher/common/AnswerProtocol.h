#pragma once

enum FromAnswerClientPacketType{
	ESendVerifyCode = 0x74,//此包是验证码
	EBadVerifyCode = 0x75,//
	EHeart = 0x76,
	EVerifyCodeUnRecognize = 0x77,
};

struct FromAnswerClientPacketHead{
	FromAnswerClientPacketType type;
	WORD totalLen;
	DWORD checkSum;
};

enum ToAnswerClientPacketType{
	EHeartEcho = 0x40,
	ESendVerifyPicture = 0x41,
};

struct ToAnswerClientPacketHead{
	ToAnswerClientPacketType type;
	WORD totalLen;
};