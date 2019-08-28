#pragma once

#include <string>
#include <vector>
#include <map>

#define LABLE			"标记"
#define JUMP_TARGET		"跳到标记"
#define IF_CONDITION	"如果"
#define THEN_DO			"那么"
#define SLEEP			"等待"
#define LISTEN_COND		"添加监视任务："
#define CLEAR_LISTREN	"清空监视"

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
	//脚本CALL名字,实际CALL
	std::map<std::string,call_info> calls_map_;
	//添加脚本函数名字-实际CALL的地址到函数映射列表中。
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
	static std::string getLableFromRoundTag(const std::string& cmd);//取得[]内的字符串
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
	/* 用于SLEEP的系列函数                                                  
	   当检测到SLEEP标志时，在SLEEP的开始到结束期间仅执行监视命令，而不执行下一条指令。
	   当监视到符合条件的条件时，要清除标志。并且跳转。
	*/
	/************************************************************************/
	bool isSleeping_;	//当前的指令序列是否被SLEEP指令阻塞。
	DWORD SleepBegin_;	//开始时间
	DWORD SleepDuring_; //持续时间
	//以下部分应为private,为了测试方便暂时设置为public
public:
	int execline(const std::string& cmd);
	//执行最简单的单条指令；
	int exec_single(const std::string& cmd);
	//执行if then指令,如果if的条件成立返回true,否则返回false
	//分离if then指令为2个部分
	bool splite_cmd_if_condition_and_exec(const std::string& cmd,std::string& condition,std::string& exec);
	bool exec_ifthen(const std::string& cmd);
	//判断if条件
	bool juge_if(const std::string& cmd);
	//判断是否是跳转指令
	bool isCmdTypeJump(const std::string& cmd);
	//执行跳转指令
	int exec_jump(const std::string& cmd);
	//添加监视
	int exec_addListen(const std::string& listen);
	//移除监视
	int exec_removeListen(const std::string& listen);
	//执行监视
	int exec_listen();
	//执行sleep
	int exec_sleep(const std::string& cmd);
	//执行清空监视
	void exec_clearListen();

	std::vector<std::string> cmds_;
	int cur_exec_line_pos_;
	std::string script_file_name_;
	std::vector<std::string> cmdOfListen_;
	int getSleepParam(const std::string& cmd); //返回-1格式错误否则为SLEEP的时间。
	int getTagPos( const std::string& tagEx );
	callmap callmap_internal;
	function_proxy funProxy_;
};
