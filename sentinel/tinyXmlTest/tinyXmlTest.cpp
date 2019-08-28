// tinyXmlTest.cpp : �������̨Ӧ�ó������ڵ㡣
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
{//��ȡӦ�ó����Ŀ¼
	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	CString strModulePath(modulePath);
	strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
	return strModulePath;
}

bool CreateXmlFile(string& szFileName)
{//����xml�ļ�,szFilePathΪ�ļ������·��,�������ɹ�����true,����false
	try
	{
		//����һ��XML���ĵ�����
		TiXmlDocument *myDocument = new TiXmlDocument();
		//����һ����Ԫ�ز����ӡ�
		TiXmlElement *RootElement = new TiXmlElement("Persons");
		myDocument->LinkEndChild(RootElement);
		//����һ��PersonԪ�ز����ӡ�
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		RootElement->LinkEndChild(PersonElement);
		//����PersonԪ�ص����ԡ�
		PersonElement->SetAttribute("ID", "1");
		//����nameԪ�ء�ageԪ�ز����ӡ�
		TiXmlElement *NameElement = new TiXmlElement("name");
		TiXmlElement *AgeElement = new TiXmlElement("age");
		PersonElement->LinkEndChild(NameElement);
		PersonElement->LinkEndChild(AgeElement);
		//����nameԪ�غ�ageԪ�ص����ݲ����ӡ�
		TiXmlText *NameContent = new TiXmlText("������");
		TiXmlText *AgeContent = new TiXmlText("22");
		NameElement->LinkEndChild(NameContent);
		AgeElement->LinkEndChild(AgeContent);

		char szCurPath[MAX_PATH] = {0};
		GetCurrentDirectoryA(MAX_PATH,szCurPath);
		string seperator = "\\";
		string fullPath = string(szCurPath) +seperator+szFileName;
		printf("%s\n",fullPath.c_str());
		myDocument->SaveFile(fullPath.c_str());//���浽�ļ�
	}
	catch (string& e)
	{
		return false;
	}
	return true;
}

bool ReadXmlFile(string& szFileName)
{//��ȡXml�ļ���������
	try
	{
		char szCurPath[MAX_PATH] = {0};
		GetCurrentDirectoryA(MAX_PATH,szCurPath);
		string seperator = "\\";
		string fullPath = string(szCurPath) +seperator+szFileName;
		//����һ��XML���ĵ�����
		TiXmlDocument *myDocument = new TiXmlDocument(fullPath.c_str());
		myDocument->LoadFile();
		//��ø�Ԫ�أ���Persons��
		TiXmlElement *RootElement = myDocument->RootElement();
		//�����Ԫ�����ƣ������Persons��
		cout << RootElement->Value() << endl;
		//��õ�һ��Person�ڵ㡣
		TiXmlElement *FirstPerson = RootElement->FirstChildElement();
		//��õ�һ��Person��name�ڵ��age�ڵ��ID���ԡ�
		TiXmlElement *NameElement = FirstPerson->FirstChildElement();
		TiXmlElement *AgeElement = NameElement->NextSiblingElement();
		TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();
		//�����һ��Person��name���ݣ��������ǣ�age���ݣ�����ID���ԣ�����
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