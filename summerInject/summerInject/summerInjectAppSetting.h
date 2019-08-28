#pragma once
#include "util/singleton.h"
#include <string>
using std::string;
#include <map>
using std::map;
class TiXmlElement;
class summerInjectAppSetting : public Singleton<summerInjectAppSetting>
{
public:
	summerInjectAppSetting();
	~summerInjectAppSetting();
public:
	string getAppSetting(const string& settingKey);
	void setAppSetting(const string& settingKey,const string& settingValue);
	void save();
protected:
private:
	void load();
	void saveChangedValue( const string& key,TiXmlElement* settingSection );
	map<string,string> _settingMap;
	map<string,string> _originalMap;
};
