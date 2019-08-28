#pragma once

#include <string>
using std::string;

#define BAD_GAME_ID		-1

class type_game_settings
{
public:
	int _game_type_id;			//<游戏类型的数字id
	string _game_type_describe;	//<游戏类型的描述
	string _acc_get_stage;		//<派发账号的策略
	string _acc_file_sub_dir;	//<各游戏对应的保存订单处理过程、结果的文件目录
	bool _is_enable_imm_acc;	//<直接添加ORDER到列表
	bool _is_can_muti_client;	//<是否多开
	int _default_max_client;	//<默认最大多开数
	string _order_table_name;	//<服务器保存从远程接口获得的账号的临时表的名字
	string _record_table_name;	//<服务器保存交易结果的表的名字
	string _confirm_table_name;
	int _def_get_db_count;		//<每次从服务器得到的数量
	int _default_order_retry_times;		//< 当订单发生非致命错误时，允许的重试次数。
	int _data_souce_query_retry_time;	//<当数据源无订单时，下次重试获取订单操作的等待毫秒数。
	string _order_log_file_path;	//<当使用订单文本记录时，获取的记录文件保存路径。
	bool _is_enable_individual_text_log;	//<是否使用独立文本记录每一个订单的简单处理过程。
protected:
private:
};