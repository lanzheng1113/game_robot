#pragma once
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "util/Singleton.h"
#include "type_game_setting.h"

/**
 * @class CSettings ����������
 */
class CSettings : public Singleton<CSettings>
{
public:
	CSettings(){_is_loaded = false;}

	BOOL initGamersConfigure();										//<��ʼ��������Ϸ������

	int getSettingDefaultMaxOrderCount(int game_type_id);			//<�������ж�ȡ��Ӧ��ϷID����Ķ࿪�������ֵ

	int getDefGetOrderCountFromDbPerBatch(int game_type_id);		//<�������ж�ȡ��Ӧ��ϷIDÿ�δӷ�������ȡ�Ķ�������

	int getDefOrderRetryTime(int game_type_id);						//< �������ж�ȡ��Ӧ��Ϸ�Ķ����ڷ�������ʱ��������Դ���

	vector<type_game_settings> getSettingsGameTypes() const;		//<��������������Ϣ

	int orderDispatchStageFromString(const string& strDispatchStage) const; //< ������������Դ��ַ�����ת��Ϊenum����

	int getDataSouceQueryRetryTime(int game_type_id);				//<��õ�����Դ������ʱ�´β�ѯ�ĳ�ʱʱ�䣬��λΪms

	string getDefOrderLogPath(int game_type_id);					//<��ʹ�÷����ı���¼ʱ���������ж�ȡ��Ϸ�Ķ�����¼����·����

	bool getIsEnableIndividualTextLog(int game_type_id);			//<�Ƿ�ʹ�ö������ı���¼��
protected:
private:
	
private:
	bool _is_loaded;//<�����Ƿ��Ѿ�����ȡ�ı�־
	vector<type_game_settings> _settings_game_support;	//����
};