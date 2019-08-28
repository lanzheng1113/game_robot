#ifndef _DVDT_MESSAGE_DEF_H
#define _DVDT_MESSAGE_DEF_H

/*主类型*/
#define		NETMESG_TYPE_MAIN_LG_DVDT		0xC005//登陆验证类型
/*登陆验证请求消息定义：第一到第十步。子类型为1到10*/
#define		NETMESG_LGDVDT_FIRSTKNOCK		(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0001/*登陆验证网络请求--第一步：FirstKnock消息*//*FirstKnock消息等于第一步的消息*/
#define		NETMESG_LGDVDT_1ST				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0001/*登陆验证网络请求--第一步：FirstKnock消息*/
#define		NETMESG_LGDVDT_2ND				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0002/*登陆验证网络请求--第二步*/
#define		NETMESG_LGDVDT_3RD				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0003/*登陆验证网络请求--第三步*/
#define		NETMESG_LGDVDT_4TH				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0004/*登陆验证网络请求--第四步*/
#define		NETMESG_LGDVDT_5TH				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0005/*登陆验证网络请求--第五步*/
#define		NETMESG_LGDVDT_6TH				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0006/*登陆验证网络请求--第六步*/
#define		NETMESG_LGDVDT_7TH				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0007/*登陆验证网络请求--第七步*/
#define		NETMESG_LGDVDT_8TH				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0008/*登陆验证网络请求--第八步*/
#define		NETMESG_LGDVDT_9TH				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x0009/*登陆验证网络请求--第九步*/
#define		NETMESG_LGDVDT_10TH				(NETMESG_TYPE_MAIN_LG_DVDT<<16) | 0x000A/*登陆验证网络请求--第十步*/


#define error_user_already_online	65534
#define error_user_not_exist		65533
#define error_user_not_usable		65532
#define error_user_timeout			65531
#define error_pswd_incorrect		65530
#define error_game_type_incorrect	65529
#define error_unknow_wg_version		65528

//代理创建账号失败
enum{
	msg_proxy_create_acc_failed = 400,
	error_proxy_create_acc_exist = 65430,
	error_proxy_create_acc_unknow,
	error_proxy_create_acc_not_enough_money,
	error_proxy_create_acc_beyond_limit,
	error_proxy_create_acc_not_enough_test_kard,
	error_proxy_create_acc_oper_failed,
	error_proxy_create_acc_too_many,
	error_proxy_acc_count_error,
	error_proxy_acc_count_cannot_zero,
};

enum{
	error_proxy_charge_acc_too_many = 65000,
	error_proxy_charge_count_cannot_zero,
	error_proxy_charge_count_error,
	error_proxy_charge_acc_failed,
	error_proxy_charge_cannot_testkard,
	error_proxy_charge_acc_not_enough_money,
	error_proxy_charge_not_your_kard,
	error_proxy_charge_acc_unknow,
};

#define msg_query_deadline					0xC0040001
#define msg_getuserlist						0xC0040002	//代理客户端获取账号列表
#define msg_unbind_one_user					0xC0040003
#define msg_change_user_proxyname			0xC0040004
#define msg_proxy_login_chk_acc_pwd			0xC0040005	//代理客户端登陆验证
#define msg_proxy_switch_acc				0xC0040006
#define msg_proxy_query_myinfo				0xC005000D
#define msg_proxy_del_users					0xC005000E
#define msg_proxy_del_dead_accs				0xC005000F //删除过期账号
#define msg_proxy_set_clientname			0xC0050010 //设置终端用户名
#define msg_proxy_disable_accs				0xC0050011 //禁用
#define msg_proxy_enable_accs				0xC0060012 //启用
#define msg_proxy_charge_accs				0xC0070013 //充值
#define msg_proxy_get_history				0xC0070014

#define msg_change_many_usersnames_enable	0xC0050007
#define msg_query_manyusers_deadline		0xC0050008
#define msg_Proxy_heart						0xC0050009	//代理客户端心跳
#define msg_proxy_create_acc				0xC005000A	//代理客户端创建账号
#define msg_proxy_update_acc_info			0xC005000B	//代理客户端更新账号信息
#define msg_proxy_ack_more_data				0xC005000C	//代理客户端回应，需要更多数据。
#endif