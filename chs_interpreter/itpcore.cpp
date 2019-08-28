#include "StdAfx.h"
#include "itpcore.h"
#include <fstream>
#include "util/StringEx.h"
#include "util/StringList.h"
#include "debugmessage.h"

void CMessageBoxA(char * szMsg,...)
{
	va_list va_args;
	int len;
	char * buffer;

	va_start(va_args,szMsg);
	//�ú������ظ�ʽ������ַ������ȣ�������Terminating Null
	len=_vscprintf(szMsg,va_args)+1;		

	buffer=new char[len*sizeof(char)];
	if(!buffer)
	{
		OutputDebugStringA("���������Ϣ���󣬷���ռ�ʧ��\n");
		return;
	}
	vsprintf_s(buffer,len,szMsg,va_args);
	MessageBoxA(NULL,buffer,NULL,MB_OK);
	//OutputDebugStringA(buffer);
	delete[] buffer;
}


int callmap::addcall( const std::string& script_call,FunctionType funtype,void* funaddr,void* objthis )
{
	int iret = -1;
	switch(funtype)
	{
	case funtype_c:
		iret = add_call_c(script_call,funaddr);
		break;
	case funtype_std:
		iret = add_call_std(script_call,funaddr);
		break;
	case funtype_this:
		iret = add_call_this(script_call,funaddr,objthis);
		break;
	default:
		break;
	}
	return iret;
}

int callmap::add_call_c( const std::string& script_call,void* fun_addr )
{
	call_info ci;
	memset(&ci,0,sizeof(ci));
	ci.fun_addr = fun_addr;
	ci.funtype = funtype_c;
	calls_map_.insert(make_pair(script_call,ci));
	return calls_map_.size();
}

int callmap::add_call_std( const std::string& script_call,void* fun_addr )
{
	call_info ci;
	memset(&ci,0,sizeof(ci));
	ci.fun_addr = fun_addr;
	ci.funtype = funtype_std;
	calls_map_.insert(make_pair(script_call,ci));
	return calls_map_.size();
}

int callmap::add_call_this( const std::string& script_call,void* fun_addr,void* objthis )
{
	call_info ci;
	memset(&ci,0,sizeof(ci));
	ci.fun_addr = fun_addr;
	ci.funtype = funtype_std;
	ci.objthis = objthis;
	calls_map_.insert(make_pair(script_call,ci));
	return calls_map_.size();
}

callmap::call_info callmap::getcall( const std::string& script_call )
{
	call_info ci;
	memset(&ci,0,sizeof(ci));
	std::map<std::string,call_info>::const_iterator it = calls_map_.find(script_call);
	if (it != calls_map_.end()){
		DebugMessage("�ҵ���%s��Ӧ�ĺ�����ַ0x%x",script_call.c_str(),(int)it->second.fun_addr);
		ci = it->second;
	}
	return ci;
}


itpcore::itpcore(void)
{
}

itpcore::~itpcore(void)
{
}

int itpcore::load()
{
	int icount = 0;
	std::ifstream ifs;
	ifs.open(script_file_name_.c_str());
	if (ifs.is_open())
	{
		std::string oneline;
		while (getline(ifs,oneline))
		{
			String strEx(oneline);
			if (strEx.beginsWith(";")){
				DebugMessage("##get one comment line## {%s}",oneline.c_str());
				continue;
			}else{
				String ex(oneline) ;
				ex = ex.trim();
				cmds_.push_back(ex);
				icount ++;
			}
		}
		ifs.close();
	}else{
		return -1;
	}
	init_call_table();
	return icount;
}

bool itpcore::splite_cmd_if_condition_and_exec( const std::string& cmd,std::string& condition,std::string& exec )
{
	//��� [�ж����� ս��״̬ >= 0x10 0] ��ô ������� [ ����������͵Ϯ�� ]
	String cmdEx(cmd);
	cmdEx = cmdEx.trim();
	if (!cmdEx.beginsWith(IF_CONDITION))
	{
		DebugMessage("��������%sʧ��,����\'%s\'��ʼ",cmd.c_str(),IF_CONDITION);
		return false;
	}


	//��ʼ����������ִ��
	std::string::const_iterator itb = cmdEx.begin();
	std::string::const_iterator ite = cmdEx.end();
	std::string::const_iterator itFirstBegin = cmdEx.end();
	std::string::const_iterator itFirstEnd = cmdEx.end();
	//find first []
	bool bfind1ststart = false;
	bool bfind1stend = false;
	for (; itb!=ite; ++itb)
	{
		if (!bfind1ststart)
		{
			if (*itb == '[')
			{
				bfind1ststart = true;
				itFirstBegin = itb;
			}
		}
		else{
			if (!bfind1stend)
			{
				if (*itb == ']')
				{
					bfind1stend = true;
					itFirstEnd = itb;
				}
			}else{
				break;
			}
		}
	}

	if ( itFirstBegin == cmdEx.end()
		|| itFirstEnd == cmdEx.end()
		)
	{
		DebugMessage("����%sʧ���ˣ���Ϊ��ȡ[����]ʧ����",cmd.c_str());
		return false;
	}else{
		bool skipfirst = false;
		while(itFirstBegin != itFirstEnd){
			if (!skipfirst)
			{
				skipfirst = true;
				itFirstBegin ++;
				continue;
			}
			condition.push_back(*itFirstBegin);
			itFirstBegin ++;
		}
		
		/*
		int pos = listen.find(LISTEN_COND);
		if (pos == std::string::npos)
		{
		return -1;
		}else{
		std::string cond = listen.substr(pos+strlen(LISTEN_COND),listen.length()-strlen(LISTEN_COND));
		cond = String(cond).trim();
		DebugMessage("ѹ���������%s",cond.c_str());
		cmdOfListen_.push_back(cond);
		cur_exec_line_pos_++;
		}
		*/
		int pos = cmd.find(THEN_DO);
		if (pos == std::string::npos)
		{
			DebugMessage("û���ҵ� %s,��%s��",THEN_DO,cmd.c_str());
			return false;
		}
		else
		{
			String thendo = cmd.substr(pos+strlen(THEN_DO),cmd.length()-strlen(THEN_DO));
			exec = thendo.trim();
		}
	}
	
	DebugMessage("���������=%s,ִ��=%s",condition.c_str(),exec.c_str());
	return true;
}

bool itpcore::exec_ifthen(const std::string& cmd)
{
	std::string cmd_condition;
	std::string cmd_exec;

	splite_cmd_if_condition_and_exec(cmd,cmd_condition,cmd_exec);

	if (juge_if(cmd_condition))
	{
		DebugMessage("====�ж��������Ϊ ���桿====");
		if (isCmdTypeJump(cmd_exec))
		{
			exec_jump(cmd_exec);
		}else{
			exec_single(cmd_exec);
		}
	}
	else
	{
		DebugMessage("====�ж��������Ϊ ���١�====");
		cur_exec_line_pos_ ++;
	}
	return false;
}

int itpcore::exec()
{
	while(1)
	{
		if (cur_exec_line_pos_ >= cmds_.size())
		{
			cur_exec_line_pos_ = 0;
		}
		String cmdEx(cmds_[cur_exec_line_pos_]);

		DebugMessage("[exec] line = %d,cmd = %s",cur_exec_line_pos_,cmdEx.c_str());

		if(cmdEx.beginsWith(LABLE))
		{	
			cur_exec_line_pos_ ++;
			DebugMessageA("[���������] : %s",cmdEx.c_str());
			continue;
		}

		exec_listen();
		DebugMessage("[ִ������] %s",cmds_[cur_exec_line_pos_].c_str());
		execline(cmds_[cur_exec_line_pos_]);
	}
	return 1;
}

#define Error_msg "##�����ָ�� %s##"

int itpcore::execline( const std::string& cmd )
{
	/*
	#define LABLE			"���"
	#define JUMP_TARGET		"�������"
	#define IF_CONDITION	"���"
	#define THEN_DO			"��ô"
	#define SLEEP			"�ȴ�"
	#define LISTEN_COND		"��Ӽ�������"
	*/
	String cmdEx(cmd);
	if (cmdEx.beginsWith(LABLE))
	{
		//����һ����־�У���������
		cur_exec_line_pos_ ++;
		return 0;
	}
	else if (cmdEx.beginsWith(JUMP_TARGET))
	{
		exec_jump(cmdEx);
	}
	else if (cmdEx.beginsWith(SLEEP))
	{
		exec_sleep(cmd);
		return 0;
	}
	else if (cmdEx.beginsWith(LISTEN_COND))
	{
		exec_addListen(cmd);
		return 0;
	}
	else if (cmdEx.beginsWith(IF_CONDITION))
	{
		exec_ifthen(cmdEx);
		return 0;
	}
	else if (cmdEx.beginsWith(CLEAR_LISTREN))
	{
		exec_clearListen();
	}
	else
	{
		exec_single(cmdEx);
	}

	return 0;
}

int itpcore::getSleepParam( const std::string& cmd )
{
	String cmdEx(cmd);
	int beginPos = cmdEx.find('[');
	int endPos = cmdEx.find(']');
	if (endPos <= beginPos){
		return -1;
	}else{
		cmdEx = cmdEx.substr(beginPos+1,endPos-beginPos-1);
		DebugMessage("sleep �ַ�����ȡ = %s",cmdEx.c_str());
		cmdEx = cmdEx.trim();
		int sleeptime = atoi(cmdEx.c_str());
		if (sleeptime <= 0)
		{
			return -1;
		}else{
			return sleeptime;
		}
	}
}

int itpcore::exec_sleep( const std::string& cmd )
{
	DebugMessage("ִ��exec_sleep");
	//��ǰִ�е���һ��SLEEPָ�
	if (!isSleeping_)
	{
		//�����ǰ��δ��SLEEP����
		//1�����SLEEPָ��ĸ�ʽ�Ƿ�Ϸ����������������Ϣ��ִ����һ����
		int sleepdur = getSleepParam(cmd);
		if (sleepdur == -1){
			DebugMessage("���SLEEPָ��ĸ�ʽ�Ƿ�Ϸ����������������Ϣ��ִ����һ����");
			DebugMessage(Error_msg,cmd.c_str());
			cur_exec_line_pos_ ++;
			return 0;
		}else{
			//2����ȡָ��ĳ���ʱ�䡣����SLEEP��־������ָ�����е�ִ�С�[cur_exec_line_pos_����]
			isSleeping_ = true;
			SleepBegin_ = GetTickCount();
			SleepDuring_ = sleepdur;
			DebugMessageA("2����ȡָ��ĳ���ʱ�䡣����SLEEP��־������ָ�����е�ִ�С�[cur_exec_line_pos_����]");
		}
	}else{
		//��ǰ���ڱ�SLEEPָ������,��ô���BLOCK��ʱ���Ƿ��Ѿ����ˡ�������˾��˳���
		DebugMessageA("��ǰ���ڱ�SLEEPָ������,��ô���BLOCK��ʱ���Ƿ��Ѿ����ˡ�������˾��˳���");
		Sleep(100);
		if (GetTickCount() - SleepBegin_ > SleepDuring_){
			//���sleep������״̬��
			isSleeping_ = false;
			cur_exec_line_pos_ ++;
		}
	}
	return 0;
}

int itpcore::exec_addListen( const std::string& listen )
{
	int pos = listen.find(LISTEN_COND);
	if (pos == std::string::npos)
	{
		return -1;
	}else{
		std::string cond = listen.substr(pos+strlen(LISTEN_COND),listen.length()-strlen(LISTEN_COND));
		cond = String(cond).trim();
		DebugMessage("ѹ���������%s",cond.c_str());
		cmdOfListen_.push_back(cond);
		cur_exec_line_pos_++;
	}
	return 0;
}

int itpcore::exec_listen()
{
	for (int i=0; i!=cmdOfListen_.size(); i++)
	{
		DebugMessage("[ִ�м���] %s",cmdOfListen_[i].c_str());
		if(exec_ifthen(cmdOfListen_[i]))
			break;
	}
	return 0;
}


//��ʱû��ʵ�ֵĴ��붼��������
bool itpcore::juge_if( const std::string& cmd )
{
	DebugMessage("ִ������%s���жϷ���ֵ",cmd.c_str());
	String cmdEx(cmd);
	cmdEx = cmdEx.trim();
	StringList sl = cmdEx.split(" ");
	if (sl.empty())
	{
		DebugMessage("ִ��%sʱ�����޷�ʶ�������",cmd.c_str());
		return false;
	}
	callmap::call_info ci = callmap_internal.getcall(sl[0]);
	if (ci.fun_addr == 0){
		CMessageBoxA("����û���ҵ�ʵ��,��ִ��%sʱ",cmd.c_str());
	}else{
		std::vector<std::string> vecParam;
		for (int i=0; i!=sl.size(); i++)
		{
			if (0 == i)
			{
				continue;
			}
			vecParam.push_back(sl[i]);
		}
		bool bxx = funProxy_.bool_function_proxy(ci,vecParam);
		return bxx;
	}
	return false;
}

bool itpcore::isCmdTypeJump( const std::string& cmd )
{
	//������� [ 9X��ʼ_SP-2���� ]
	String cmdEx(cmd);
	cmdEx = cmdEx.trim();
	if (!cmdEx.beginsWith(JUMP_TARGET))
	{
		return false;
	}
	return true;
}

int itpcore::exec_jump(const std::string& cmd)
{
// ������� [ ������Ʒ ] 
	std::string tag = stringtool::getLableFromRoundTag(cmd);
	String tagEx(tag);
	tagEx = tagEx.trim(); //������Ʒ
	int tagPos = getTagPos(tagEx);
	if (tagPos == -1)
	{
		CMessageBoxA("û���ҵ���� %s",tagEx.c_str());
		cur_exec_line_pos_ ++;
	}else{
		DebugMessage("[ִ����ת] Ŀ���� = %s��Ŀ���� = %d",tagEx.c_str(),cur_exec_line_pos_);
		cur_exec_line_pos_ = tagPos;
	}
	return 0;
}

int itpcore::exec_single(const std::string& cmd)
{
	String cmdEx(cmd);
	cmdEx = cmdEx.trim();
	cmdEx.remove(",");
	StringList sl = cmdEx.split(" ");
	if (sl.empty())
	{
		cur_exec_line_pos_ ++;
		return 0;
	}
	callmap::call_info ci = callmap_internal.getcall(sl[0]);
	if (ci.fun_addr == 0)
	{
		CMessageBoxA("����û���ҵ�ʵ��,��ִ��%sʱ",cmd.c_str());
		cur_exec_line_pos_ ++;
		return 0;
	}

	String params = stringtool::getLableFromRoundTag(cmd);
	params = params.trim();
	StringList slParams = params.split(" ");

	std::vector<std::string> vecParam;
	for (int i=0; i!=slParams.size(); i++)
	{
		vecParam.push_back(slParams[i]);
	}
	funProxy_.void_function_proxy(ci,vecParam);
	cur_exec_line_pos_ ++;
	return 0;
}

int itpcore::getTagPos( const std::string& tagEx )
{
	for (int i = 0; i!=cmds_.size(); i++)
	{
		String cmdEx(cmds_[i]);
		cmdEx = cmdEx.trim();
		if (cmdEx.beginsWith(LABLE))//���
		{
			String tagtemp = stringtool::getLableFromRoundTag(cmdEx);
			tagtemp = tagtemp.trim();
			if (tagtemp == tagEx)
			{
				DebugMessage("�ҵ��˱��:%s,λ��=%d",tagEx.c_str(),i);
				return i;
			}
		}
	}
	return -1;
}

void itpcore::exec_clearListen()
{
	DebugMessage("[��ռ���]");
	cmdOfListen_.clear();
	cur_exec_line_pos_++;
	return;
}

bool function_proxy::bool_function_proxy( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	/*	funtype_c,
	funtype_this,
	funtype_std,*/
	switch (ci.funtype)
	{
	case funtype_c:
		return bool_c_fun(ci,params);
	case funtype_this:
		return bool_this_fun(ci,params);
	case funtype_std:
		return bool_std_fun(ci,params);
	default:
		break;
	}
	return false;
}

bool function_proxy::bool_c_fun( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	void* calladdr = ci.fun_addr;
	void* callParam = (void*)&params;
	DWORD retvalue = 0;
	__asm{
		push callParam
		call calladdr
		mov retvalue,eax
		add esp,4
	}
	return retvalue != 0;
}

bool function_proxy::bool_std_fun( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	void* calladdr = ci.fun_addr;
	void* callParam = (void*)&params;
	DWORD retvalue = 0;
	__asm{
		push callParam
		call calladdr
		mov retvalue,eax
	}
	return retvalue != 0;
}

bool function_proxy::bool_this_fun( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	void* calladdr = ci.fun_addr;
	void* callParam = (void*)&params;
	DWORD retvalue = 0;
	DWORD callthis = (DWORD)ci.objthis;
	__asm{
		mov ecx,callthis
		push callParam
		call calladdr
		mov retvalue,eax
	}
	return retvalue != 0;
}

void function_proxy::void_c_fun( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	DWORD calladdr = (DWORD)ci.fun_addr;
	DWORD callParam = (DWORD)&params;
	__asm{
		push callParam
		call calladdr
		add esp,4
	}
	return;
}

void function_proxy::void_std_fun( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	DWORD calladdr = (DWORD)ci.fun_addr;
	DWORD callParam = (DWORD)&params;
	__asm{
		push callParam
		call calladdr
	}
	return;
}

void function_proxy::void_this_fun( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	DWORD calladdr = (DWORD)ci.fun_addr;
	DWORD callParam = (DWORD)&params;
	DWORD callthis = (DWORD)ci.objthis;

	__asm{
		mov ecx,callthis
		push callParam
		call calladdr
	}
	return;
}

void function_proxy::void_function_proxy( const callmap::call_info& ci,const std::vector<std::string>& params )
{
	switch (ci.funtype)
	{
	case funtype_c:
		void_c_fun(ci,params);
		break;
	case funtype_this:
		void_this_fun(ci,params);
		break;
	case funtype_std:
		void_std_fun(ci,params);
		break;
	default:
		break;
	}
	return;
}

std::string stringtool::getLableFromRoundTag( const std::string& cmd )
{
	std::string ret;
	bool isfoundleft = false;
	bool isfoundright = false;
	for (int i=0; i!=cmd.size(); i++)
	{
		if (isfoundright)
		{
			break;
		}

		if (!isfoundleft)
		{
			if (cmd[i] == '[')
			{
				isfoundleft = true;
			}
		}else{
			if (cmd[i] == ']')
			{
				isfoundright = true;
				break;
			}else{
				ret.push_back(cmd[i]);
			}
		}
	}

	return ret;
}
