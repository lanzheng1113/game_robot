#ifndef _EVENT_ID_DEF_H
#define _EVENT_ID_DEF_H
													//事件类别ID
#define Event_caledge_runtime_error				0x07		//保留，用于程序异常捕捉
#define Event_caledge_DoValidate_fail			0x08		//验证失败信息
#define Event_caledge_commom					0x09		//普通信息

													//事件ID
#define Eid_Client_SystemInfo_is_NULL			0x00c1		//用户机器信息为空
#define Eid_3Second_Login_TimeOut				0x00c2		//登录时3秒违例
#define Eid_UserLoginSuccess					0x00c3		//用户登录成功
#define Eid_DoValidate_SystemInfo_Incorrect		0x00c4		//绑定错误
//#define Eid_DoValidate_1st_UserCrash			0x00c5		//用户执行thdini崩溃
#define Eid_DoValidate_2nd_UserCrash			0x00c6		//用户执行getidentify崩溃
//#define Eid_DoValidate_3rd_UserCrash			0x00c7		//用户执行第三步，赋偏移时发生崩溃
#define Eid_UserAlreayOnline					0x00c8

#endif