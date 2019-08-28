// tinyXmlTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../tinyxml/tinystr.h"
#include "../tinyxml/tinyxml.h"

#include <iostream>
#include <string>
//#include <windows.h>
#include <atlstr.h>
using namespace std;

CString GetAppPath()
{//获取应用程序根目录
	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	CString strModulePath(modulePath);
	strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
	return strModulePath;
}

bool CreateXmlFile(string& szFileName)
{//创建xml文件,szFilePath为文件保存的路径,若创建成功返回true,否则false
	try
	{
		//创建一个XML的文档对象。
		TiXmlDocument *myDocument = new TiXmlDocument();
		//创建一个根元素并连接。
		TiXmlElement *RootElement = new TiXmlElement("Persons");
		myDocument->LinkEndChild(RootElement);
		//创建一个Person元素并连接。
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		RootElement->LinkEndChild(PersonElement);
		//设置Person元素的属性。
		PersonElement->SetAttribute("ID", "1");
		//创建name元素、age元素并连接。
		TiXmlElement *NameElement = new TiXmlElement("name");
		TiXmlElement *AgeElement = new TiXmlElement("age");
		PersonElement->LinkEndChild(NameElement);
		PersonElement->LinkEndChild(AgeElement);
		//设置name元素和age元素的内容并连接。
		TiXmlText *NameContent = new TiXmlText("周星星");
		TiXmlText *AgeContent = new TiXmlText("22");
		NameElement->LinkEndChild(NameContent);
		AgeElement->LinkEndChild(AgeContent);

		char szCurPath[MAX_PATH] = {0};
		GetCurrentDirectoryA(MAX_PATH,szCurPath);
		string seperator = "\\";
		string fullPath = string(szCurPath) +seperator+szFileName;
		printf("%s\n",fullPath.c_str());
		myDocument->SaveFile(fullPath.c_str());//保存到文件
	}
	catch (string& e)
	{
		return false;
	}
	return true;
}

bool ReadXmlFile(string& szFileName)
{//读取Xml文件，并遍历
	try
	{
		char szCurPath[MAX_PATH] = {0};
		GetCurrentDirectoryA(MAX_PATH,szCurPath);
		string seperator = "\\";
		string fullPath = string(szCurPath) +seperator+szFileName;
		//创建一个XML的文档对象。
		TiXmlDocument *myDocument = new TiXmlDocument(fullPath.c_str());
		myDocument->LoadFile();
		//获得根元素，即Persons。
		TiXmlElement *RootElement = myDocument->RootElement();
		//输出根元素名称，即输出Persons。
		cout << RootElement->Value() << endl;
		//获得第一个Person节点。
		TiXmlElement *FirstPerson = RootElement->FirstChildElement();
		//获得第一个Person的name节点和age节点和ID属性。
		TiXmlElement *NameElement = FirstPerson->FirstChildElement();
		TiXmlElement *AgeElement = NameElement->NextSiblingElement();
		TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();
		//输出第一个Person的name内容，即周星星；age内容，即；ID属性，即。
		cout << "FirstPerson->value" << FirstPerson->Value() <<endl;
		cout << NameElement->FirstChild()->Value() << endl;
		cout << AgeElement->FirstChild()->Value() << endl;
		cout << IDAttribute->Value()<< endl;
	}
	catch (string& e)
	{
		return false;
	}
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	string fileName = "info.xml";
	CreateXmlFile(fileName);
	ReadXmlFile(fileName);
	return 0;
}