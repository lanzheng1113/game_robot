#pragma once
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "util/Singleton.h"
#include "type_game_setting.h"

/**
 * @class CSettings 服务器配置
 */
class CSettings : public Singleton<CSettings>
{
public:
	CSettings(){_is_loaded = false;}

	BOOL initGamersConfigure();										//<初始化交易游戏的配置

	int getSettingDefaultMaxOrderCount(int game_type_id);			//<从配置中读取对应游戏ID允许的多开数量最大值

	int getDefGetOrderCountFromDbPerBatch(int game_type_id);		//<从配置中读取对应游戏ID每次从服务器读取的订单数量

	int getDefOrderRetryTime(int game_type_id);						//< 从配置中读取对应游戏的订单在发生错误时允许的重试次数

	vector<type_game_settings> getSettingsGameTypes() const;		//<返回整个配置信息

	int orderDispatchStageFromString(const string& strDispatchStage) const; //< 将订单分配策略从字符描述转换为enum描述

	int getDataSouceQueryRetryTime(int game_type_id);				//<获得当数据源无数据时下次查询的超时时间，单位为ms

	string getDefOrderLogPath(int game_type_id);					//<当使用分立文本记录时，从配置中读取游戏的订单记录保存路径。

	bool getIsEnableIndividualTextLog(int game_type_id);			//<是否使用独立的文本记录。
protected:
private:
	
private:
	bool _is_loaded;//<配置是否已经被读取的标志
	vector<type_game_settings> _settings_game_support;	//配置
};