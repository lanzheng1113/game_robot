#pragma once

#include <string>
using std::string;

#define BAD_GAME_ID		-1

class type_game_settings
{
public:
	int _game_type_id;			//<��Ϸ���͵�����id
	string _game_type_describe;	//<��Ϸ���͵�����
	string _acc_get_stage;		//<�ɷ��˺ŵĲ���
	string _acc_file_sub_dir;	//<����Ϸ��Ӧ�ı��涩��������̡�������ļ�Ŀ¼
	bool _is_enable_imm_acc;	//<ֱ�����ORDER���б�
	bool _is_can_muti_client;	//<�Ƿ�࿪
	int _default_max_client;	//<Ĭ�����࿪��
	string _order_table_name;	//<�����������Զ�̽ӿڻ�õ��˺ŵ���ʱ�������
	string _record_table_name;	//<���������潻�׽���ı������
	string _confirm_table_name;
	int _def_get_db_count;		//<ÿ�δӷ������õ�������
	int _default_order_retry_times;		//< ��������������������ʱ����������Դ�����
	int _data_souce_query_retry_time;	//<������Դ�޶���ʱ���´����Ի�ȡ���������ĵȴ���������
	string _order_log_file_path;	//<��ʹ�ö����ı���¼ʱ����ȡ�ļ�¼�ļ�����·����
	bool _is_enable_individual_text_log;	//<�Ƿ�ʹ�ö����ı���¼ÿһ�������ļ򵥴�����̡�
protected:
private:
};