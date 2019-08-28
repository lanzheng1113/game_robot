#include <string>
using std::string;
#include "InjectHistory.h"
#include "util/StringList.h"
#include "util/Path.h"
#include "util/file.h"
#include "tinyxml/tinyxml/tinyxml.h"
#include "tinyxml/tinyxml/tinystr.h"
#include <QMessageBox>
#include <QString>
#include <iostream>

using std::cout;
using std::endl;

InjectHistory::InjectHistory()
{

}

InjectHistory::~InjectHistory()
{

}

void InjectHistory::load()
{
	_historyList.clear();
	string xmlPath = Path::getApplicationDirPath();
	xmlPath += "summerInject.xml";
	if (!File::exists(xmlPath))
	{
		QMessageBox::information(NULL,QString("error"),QString("xml file not exist!"),QMessageBox::Ok);
	}
	
	TiXmlDocument* _XmlDocument = new TiXmlDocument(xmlPath.c_str());
	if (_XmlDocument == NULL)
	{
		return;
	}

	if (!_XmlDocument->LoadFile())
	{
		return;
	}
	//get root element <summerInject>
	TiXmlElement* rootElement = _XmlDocument->RootElement();
	string str = rootElement->Value();
	cout << "root element value:" << rootElement->Value() << endl;
	//get <dllinject_history>
	TiXmlElement *dll_inject_history = rootElement->FirstChildElement("dllinject_history");
	if (dll_inject_history)
	{
		//get <history> sections
		TiXmlElement *historyElement = dll_inject_history->FirstChildElement("history");
		while (historyElement)
		{
			TiXmlElement* historyValueElement = historyElement->FirstChildElement();
			_historyList.push_back(string(historyElement->GetText()));
			historyElement = historyElement->NextSiblingElement("history");
		}
	}

	delete _XmlDocument;
}

void InjectHistory::save()
{
	//first ,remove all the <history> sections
	string xmlPath = Path::getApplicationDirPath();
	xmlPath += "summerInject.xml";
	if (!File::exists(xmlPath))
	{
		QMessageBox::information(NULL,QString("error"),QString("xml file not exist!"),QMessageBox::Ok);
		return;
	}

	TiXmlDocument* _XmlDocument = new TiXmlDocument(xmlPath.c_str());
	if (_XmlDocument == NULL)
	{
		return;
	}

	if (!_XmlDocument->LoadFile())
	{
		delete _XmlDocument;
		return;
	}
	//get root element <summerInject>
	TiXmlElement* rootElement = _XmlDocument->RootElement();
	if (!rootElement)
	{
		delete _XmlDocument;
		return;
	}

	string str = rootElement->Value();
	//get <dllinject_history>
	TiXmlElement *dll_inject_history = rootElement->FirstChildElement("dllinject_history");
	if (!dll_inject_history)
	{
		delete _XmlDocument;
		return;
	}

	//get <history> sections
	TiXmlElement *historyElement;
	while (historyElement = dll_inject_history->FirstChildElement("history"))
	{
		dll_inject_history->RemoveChild(historyElement);
	}

	//then ,rewrite it;
	for (StringList::const_iterator it = _historyList.begin(); it != _historyList.end(); ++it)
	{
		string historys = *it;
		TiXmlElement* elm = new TiXmlElement("history");
		TiXmlText* txt = new TiXmlText(historys.c_str());
		elm->LinkEndChild(txt);
		dll_inject_history->LinkEndChild(elm);
	}

	//finally close the file;
	_XmlDocument->SaveFile(xmlPath.c_str());
	delete _XmlDocument;
	return;
}

StringList InjectHistory::getHistory()
{
	return _historyList;
}

void InjectHistory::add( const string& record )
{
	//如果已经有了，并且是最后一个元素。则直接返回
	if (!_historyList.empty())
	{
		if (*(_historyList.rbegin()) == record)
		{
			return;
		}
	}

	//如果已经有了，则先删除它
	for (StringList::const_iterator it = _historyList.begin(); it != _historyList.end(); ++it)
	{
		if (*it == record)
		{
			_historyList.erase(it);
			break;
		}
	}

	//放在最后面
	_historyList.push_back(record);
}

void InjectHistory::del( const string& record )
{
	for (StringList::const_iterator it = _historyList.begin(); it != _historyList.end(); ++it)
	{
		if (*it == record)
		{
			_historyList.erase(it);
			break;
		}
	}
}