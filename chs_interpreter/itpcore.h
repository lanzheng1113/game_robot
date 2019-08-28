#pragma once

#include <string>
#include <vector>
#include <map>

#define LABLE			"���"
#define JUMP_TARGET		"�������"
#define IF_CONDITION	"���"
#define THEN_DO			"��ô"
#define SLEEP			"�ȴ�"
#define LISTEN_COND		"��Ӽ�������"
#define CLEAR_LISTREN	"��ռ���"

enum FunctionType
{
	funtype_c,
	funtype_this,
	funtype_std,
};

#define VOID_PROXY_CALL
#define BOOL_PROXY_CALL
void CMessageBoxA(char * szMsg,...);
class callmap
{
public:
	typedef struct  
	{
		void* fun_addr;
		FunctionType funtype;
		void* objthis;
	}call_info;
public:
	int addcall(const std::string& script_call,FunctionType funtype,void* funaddr,void* objthis);
	call_info getcall(const std::string& script_call);
protected:
private:
	//�ű�CALL����,ʵ��CALL
	std::map<std::string,call_info> calls_map_;
	//��ӽű���������-ʵ��CALL�ĵ�ַ������ӳ���б��С�
	int add_call_c(const std::string& script_call,void* fun_addr);
	int add_call_std(const std::string& script_call,void* fun_addr);
	int add_call_this(const std::string& script_call,void* fun_addr,void* objthis);
};

class function_proxy
{
public:
	bool bool_function_proxy(const callmap::call_info& ci,const std::vector<std::string>& params);
	void void_function_proxy(const callmap::call_info& ci,const std::vector<std::string>& params);
protected:

private:
	bool bool_c_fun(const callmap::call_info& ci,const std::vector<std::string>& params);
	bool bool_std_fun(const callmap::call_info& ci,const std::vector<std::string>& params);
	bool bool_this_fun(const callmap::call_info& ci,const std::vector<std::string>& params);

	void void_c_fun(const callmap::call_info& ci,const std::vector<std::string>& params);
	void void_std_fun(const callmap::call_info& ci,const std::vector<std::string>& params);
	void void_this_fun(const callmap::call_info& ci,const std::vector<std::string>& params);
};

class stringtool
{
public:
	static std::string getLableFromRoundTag(const std::string& cmd);//ȡ��[]�ڵ��ַ���
protected:
private:
};

class itpcore
{
public:
	itpcore(void);
	~itpcore(void);
	int init_call_table();
	void setSourceFile(const std::string& fileName){ script_file_name_ = fileName;}
	int load();
	int exec();
private:
	/************************************************************************/
	/* ����SLEEP��ϵ�к���                                                  
	   ����⵽SLEEP��־ʱ����SLEEP�Ŀ�ʼ�������ڼ��ִ�м����������ִ����һ��ָ�
	   �����ӵ���������������ʱ��Ҫ�����־��������ת��
	*/
	/************************************************************************/
	bool isSleeping_;	//��ǰ��ָ�������Ƿ�SLEEPָ��������
	DWORD SleepBegin_;	//��ʼʱ��
	DWORD SleepDuring_; //����ʱ��
	//���²���ӦΪprivate,Ϊ�˲��Է�����ʱ����Ϊpublic
public:
	int execline(const std::string& cmd);
	//ִ����򵥵ĵ���ָ�
	int exec_single(const std::string& cmd);
	//ִ��if thenָ��,���if��������������true,���򷵻�false
	//����if thenָ��Ϊ2������
	bool splite_cmd_if_condition_and_exec(const std::string& cmd,std::string& condition,std::string& exec);
	bool exec_ifthen(const std::string& cmd);
	//�ж�if����
	bool juge_if(const std::string& cmd);
	//�ж��Ƿ�����תָ��
	bool isCmdTypeJump(const std::string& cmd);
	//ִ����תָ��
	int exec_jump(const std::string& cmd);
	//��Ӽ���
	int exec_addListen(const std::string& listen);
	//�Ƴ�����
	int exec_removeListen(const std::string& listen);
	//ִ�м���
	int exec_listen();
	//ִ��sleep
	int exec_sleep(const std::string& cmd);
	//ִ����ռ���
	void exec_clearListen();

	std::vector<std::string> cmds_;
	int cur_exec_line_pos_;
	std::string script_file_name_;
	std::vector<std::string> cmdOfListen_;
	int getSleepParam(const std::string& cmd); //����-1��ʽ�������ΪSLEEP��ʱ�䡣
	int getTagPos( const std::string& tagEx );
	callmap callmap_internal;
	function_proxy funProxy_;
};
