#include "summerInjectAppSetting.h"
#include "tinyxml/tinyxml/tinystr.h"
#include "tinyxml/tinyxml/tinyxml.h"
#include "util/path.h"
#include <QMessageBox>
#include <QString>
#include "include/XTrack.h"
using namespace XYSLib;

summerInjectAppSetting::summerInjectAppSetting()
{
	load();
}

summerInjectAppSetting::~summerInjectAppSetting()
{

}

string summerInjectAppSetting::getAppSetting( const string& settingKey )
{
	return _settingMap[settingKey];
}

void summerInjectAppSetting::setAppSetting( const string& settingKey,const string& settingValue )
{
	_settingMap[settingKey] = settingValue;
}

void summerInjectAppSetting::save()
{
	if (_originalMap == _settingMap)
	{
		XTRACE<<"the AppSetting is not changed,no need to save!" << dbg;
		return;
	}else{
		string strXmlPath = Path::getApplicationDirPath();
		strXmlPath += "summerInject.xml";
		TiXmlDocument *myDocument = new TiXmlDocument(strXmlPath.c_str());
		if (!myDocument)
		{
			return;
		}

		if (!myDocument->LoadFile())
		{
			delete myDocument;
			return;
		}

		TiXmlElement *root = myDocument->RootElement();
		if (!root)
		{
			delete myDocument;
			return;
		}

		TiXmlElement* settingSection = root->FirstChildElement("setting");
		if (!settingSection)
		{
			delete myDocument;
			return;
		}

		for (map<string,string>::const_iterator it = _originalMap.begin(); it != _originalMap.end(); ++it)
		{
			saveChangedValue(it->first,settingSection);
		}

		myDocument->SaveFile(strXmlPath.c_str());
		
		delete myDocument;
		return;
	}
}


void summerInjectAppSetting::saveChangedValue( const string& key,TiXmlElement* settingSection )
{
	if (_settingMap[key] != _originalMap[key])
	{
		TiXmlElement* oldElement = settingSection->FirstChildElement(key.c_str());
		oldElement->FirstChild()->ToText()->SetValue(_settingMap[key].c_str());
	}
}


void summerInjectAppSetting::load()
{
	string strXmlPath = Path::getApplicationDirPath();
	strXmlPath += "summerInject.xml";
	TiXmlDocument *myDocument = new TiXmlDocument(strXmlPath.c_str());
	if (!myDocument)
	{
		return;
	}
	
	if (!myDocument->LoadFile())
	{
		goto load_out;
	}

	TiXmlElement *root = myDocument->RootElement();
	if (!root)
	{
		goto load_out;
	}

	TiXmlElement *settingSection = root->FirstChildElement("setting");
	if (!settingSection)
	{
		goto load_out;
	}

	TiXmlElement *settings = settingSection->FirstChildElement();
	while(settings){
		string strKey = settings->Value();
		string strValue = settings->GetText();
		_settingMap.insert(make_pair(strKey,strValue));
		settings = settings->NextSiblingElement();
	}
	
	_originalMap = _settingMap;
load_out:
	delete myDocument;
	return;
//	TiXmlElement *root
}