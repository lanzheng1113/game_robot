#ifndef _ERROR_MESG_DEF_H
#define _ERROR_MESG_DEF_H


#define msg_proxy_unbind_success			0x40010001
#define msg_proxy_login_success				0x40010002
#define msg_proxy_infor						0x40010003
#define msg_waigua_price					0x40010004

#define msg_error_user_already_online		0x12345678
#define msg_error_user_not_exist			0x34125678
#define msg_error_user_not_usable			0x34127856
#define msg_error_user_time_out				0x11111111
#define msg_error_password_incorrect		0x22222222
#define msg_error_gametype_incorrect		0x33333333
#define msg_error_unknow_wg_version			0x44444444
#define msg_error_unknow_command			0x55555555
#define msg_error_bad_msg_len				0x55555556
#define msg_error_proxy_acc_pswd_incorrect	0x55555557
#define msg_error_proxy_srv_internal_error	0x55555558
#define msg_error_proxy_no_privilege		0x55555559
#define msg_error_proxy_not_your_kard		0x5555555A
#define msg_error_acc_count_cannot_zero		0x5555555B
#define msg_error_acc_count_beyond_max		0x5555555C
#define msg_error_unbind_days_limit			0x5555555D
#define msg_error_unknow_error				0x5555555E


typedef enum{
	kAccModifySuccess,
	kAccNotExist,
	kAccIsUsableAlready,
	kServerError
} ModifyProxyNameReuslt;

typedef enum{
	kUnbindAccSuccess,
	kUnbindAccNotExist,
	kUnbindServerError
} UnbindAccResult;

typedef enum{
	kGetUserListError_NotLogin,
	kGetUserListError_ProxyNotExist
}GetUserListError;
#endif