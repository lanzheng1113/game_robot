#include "stdafx.h"
#include "Settings.h"
#include "../common/tinyxml/include/tinyxml.h"
#include "../common/tinyxml/include/tinystr.h"
#include "../common/DebugMessage.h"
#include "TradeOrder.h"
#include <exception>
#include "util/Path.h"
using std::runtime_error;

BOOL CSettings::initGamersConfigure()
{
	if (_is_loaded){
		_settings_game_support.clear();
	}

	char moduleName[MAX_PATH];
	GetModuleFileNameA(NULL, moduleName, sizeof(moduleName));
	string strAppDir(moduleName);
	string::size_type pos = strAppDir.rfind('\\');

	if (pos == string::npos || pos == strAppDir.length() - 1) {
		;
	} else {
		strAppDir = strAppDir.substr(0, pos);
	}
	strAppDir += "\\games.xml";

	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument(strAppDir.c_str());
	if (!myDocument)
	{
		AfxMessageBox(L"创建XML配置对象失败！");
		return FALSE;
	}
	DebugMessageA("open ProxyClient main setting file:%s\n",strAppDir.c_str());
	//AfxMessageBox(L"创建配置对象成功！");
	if (!myDocument->LoadFile())
	{
		AfxMessageBox(L"打开配置文件games.xml失败！");
		delete myDocument;
		return FALSE;
	}

	//AfxMessageBox(L"打开配置文件ProxyClient.xml成功！");
	//获得根元素
	_settings_game_support.clear();
	TiXmlElement *RootElement = myDocument->RootElement();
	if (RootElement)
	{
		//输出根元素名称，即输出NVDSEx
		DebugMessageA("%s\n",RootElement->Value());
		if(string(RootElement->Value()) != "game_support"){
			delete myDocument;
			AfxMessageBox(L"错误的配置文件");
			return FALSE;
		}
		//获得第一个子节点。
		TiXmlElement *Element = RootElement->FirstChildElement();
		while (Element)
		{
			if(string(Element->Value()) == "dispatcher"){
				type_game_settings ccav;
				memset(&ccav,0,sizeof(ccav));

				TiXmlElement* ServerSettingElement = Element->FirstChildElement("game_type_id");
				if (ServerSettingElement)
				{
					ccav._game_type_id = atoi(ServerSettingElement->GetText());
				}else{
					ccav._game_type_id = BAD_GAME_ID;
					continue;
				}

				ServerSettingElement = Element->FirstChildElement("game_type_desc");
				if (ServerSettingElement)
				{
					ccav._game_type_describe = ServerSettingElement->GetText();
				}else{
					ccav._game_type_describe = "";
				}

				ServerSettingElement = Element->FirstChildElement("acc_get_stage");
				if (ServerSettingElement)
				{
					ccav._acc_get_stage = ServerSettingElement->GetText();
				}else{
					ccav._acc_get_stage = "";
				}

				ServerSettingElement = Element->FirstChildElement("acc_list_dir");
				if (ServerSettingElement)
				{
					ccav._acc_file_sub_dir = ServerSettingElement->GetText();
				}else{
					ccav._acc_file_sub_dir = "";
				}

				ServerSettingElement = Element->FirstChildElement("enable_imm_acc");
				if (ServerSettingElement)
				{
					ccav._is_enable_imm_acc = string(ServerSettingElement->GetText()) == "yes";
				}else{
					ccav._is_enable_imm_acc = false;
				}

				ServerSettingElement = Element->FirstChildElement("can_muti_client");
				if (ServerSettingElement)
				{
					ccav._is_can_muti_client = string(ServerSettingElement->GetText()) == "yes";
				}else{
					ccav._is_can_muti_client = false;
				}

				ServerSettingElement = Element->FirstChildElement("default_max_client");
				if (ServerSettingElement)
				{
					ccav._default_max_client = atoi(ServerSettingElement->GetText());
				}else{
					ccav._default_max_client = 1;
				}

				ServerSettingElement = Element->FirstChildElement("temp_order_table");
				if (ServerSettingElement)
				{
					ccav._order_table_name = ServerSettingElement->GetText();
				}else{
					ccav._order_table_name = "unknow table name";
					continue;
				}

				ServerSettingElement = Element->FirstChildElement("trade_record_table");
				if (ServerSettingElement)
				{
					ccav._record_table_name = ServerSettingElement->GetText();
				}else{
					ccav._record_table_name = "unknow table name";
					continue;
				}

				ServerSettingElement = Element->FirstChildElement("def_get_db_count");
				if (ServerSettingElement)
				{
					ccav._def_get_db_count = atoi(ServerSettingElement->GetText());
				}else{
					ccav._def_get_db_count = 1000;
				}
				
				ServerSettingElement = Element->FirstChildElement("data_souce_query_retry_time");
				if (ServerSettingElement)
				{
					ccav._data_souce_query_retry_time = atoi(ServerSettingElement->GetText());
				}else{
					ccav._data_souce_query_retry_time = 60000;
				}
				
				ServerSettingElement = Element->FirstChildElement("default_order_retry_times");
				if (ServerSettingElement)
				{
					ccav._default_order_retry_times = atoi(ServerSettingElement->GetText());
				}else{
					ccav._default_order_retry_times = 3;
				}

				ServerSettingElement = Element->FirstChildElement("order_log_file_path");
				if (ServerSettingElement)
				{
					ccav._order_log_file_path = ServerSettingElement->GetText();
				}else{
					string logFileName = Path::getApplicationDirPath();
					logFileName += "logs_";
					logFileName += ccav._game_type_describe;
					ccav._order_log_file_path = logFileName;
				}

				ServerSettingElement = Element->FirstChildElement("is_enable_individual_text_log");
				if (ServerSettingElement)
				{
					ccav._is_enable_individual_text_log = atoi(ServerSettingElement->GetText()) == 1 ? true : false;
				}else{
					ccav._is_enable_individual_text_log = false;
				}

				ServerSettingElement = Element->FirstChildElement("confirm_table_name");
				if (ServerSettingElement)
				{
					ccav._confirm_table_name = ServerSettingElement->GetText();
				}else{
					ccav._confirm_table_name = "";
				}

				_settings_game_support.push_back(ccav);
			}
			Element = Element->NextSiblingElement();
		}
	}
	delete myDocument;
	_is_loaded = true;
	return _settings_game_support.size() != 0;
}

int CSettings::getSettingDefaultMaxOrderCount( int game_type_id ){
	if (!_is_loaded)
	{
		throw runtime_error("配置未经装载，无法读取信息！");
	}

	for (vector<type_game_settings>::const_iterator it = _settings_game_support.begin(); it != _settings_game_support.end(); ++it){
		if (it->_game_type_id == game_type_id){
			return it->_default_max_client;
		}
	}
	return 1;
}


int CSettings::orderDispatchStageFromString(const string& strDispatchStage) const
{
	//@FIXME:在这里添加订单分配策略的描述转换
	if (strDispatchStage != "按优先级")
		return one_by_one_sort_in_time;
	else
		return	by_emergency_level;
}

vector<type_game_settings> CSettings::getSettingsGameTypes() const
{
	if (!_is_loaded)
	{
		throw runtime_error("配置未经装载，无法读取信息！");
	}
	return _settings_game_support;
}

int CSettings::getDefGetOrderCountFromDbPerBatch( int game_type_id )
{
	if (!_is_loaded)
	{
		throw runtime_error("配置未经装载，无法读取信息！");
	}

	for (vector<type_game_settings>::const_iterator it = _settings_game_support.begin(); it != _settings_game_support.end(); ++it){
		if (it->_game_type_id == game_type_id){
			return it->_def_get_db_count;
		}
	}
	return 0;
}

int CSettings::getDataSouceQueryRetryTime( int game_type_id )
{
	if (!_is_loaded)
	{
		throw runtime_error("配置未经装载，无法读取信息！");
	}

	for (vector<type_game_settings>::const_iterator it = _settings_game_support.begin(); it != _settings_game_support.end(); ++it){
		if (it->_game_type_id == game_type_id){
			return it->_data_souce_query_retry_time;
		}
	}

	return -1;
}

int CSettings::getDefOrderRetryTime( int game_type_id )
{
	if (!_is_loaded)
	{
		throw runtime_error("配置未经装载，无法读取信息！");
	}

	for (vector<type_game_settings>::const_iterator it = _settings_game_support.begin(); it != _settings_game_support.end(); ++it){
		if (it->_game_type_id == game_type_id){
			return it->_default_order_retry_times;
		}
	}
	return 0;
}

string CSettings::getDefOrderLogPath( int game_type_id )
{
	if(!_is_loaded){
		throw runtime_error("配置未经装载，无法读取信息");
	}
	for (vector<type_game_settings>::const_iterator it = _settings_game_support.begin(); it!=_settings_game_support.end();++it)
	{
		if (it->_game_type_id == game_type_id)
		{
			return it->_order_log_file_path;
		}
	}
	return string("");
}

bool CSettings::getIsEnableIndividualTextLog( int game_type_id )
{
	if (!_is_loaded)
	{
		throw runtime_error("配置未经装载，无法读取信息");
	}

	for (vector<type_game_settings>::const_iterator it = _settings_game_support.begin(); it!=_settings_game_support.end();++it)
	{
		if (it->_game_type_id == game_type_id)
		{
			return it->_is_enable_individual_text_log;
		}
	}
	return false;
}
