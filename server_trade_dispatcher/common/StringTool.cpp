#include "stdafx.h"
#include "StringTool.h"
#include "windows.h"

void StringTrimBlankW( wchar_t* str )
{
	int len = wcslen(str);
	for (int i=0; i!=len;i++)
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n')
		{
			if (i==len-1){
				str[i] = NULL;
			}else{
				wcscpy(str+i,str+i+1);
			}
			len --;
			i --;
		}
	}
}

CString StringToCString( const string& str )
{
	int len = str.length();
	if (len == 0)
	{
		return CString();
	}

	wchar_t *temp = new wchar_t[len+1];
	if (!temp){
		return CString();
	}
	
	MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,temp,len+1);

	CString strRet(temp);

	delete [] temp;
	
	return strRet;
}

string CStringToString( const CString& str )
{
	int len = str.GetLength();
	if (len == 0)
		return string();
	
	char *temp = new char[2*len+1];
	if (!temp)
		return string();
	
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,temp,2*len+1,NULL,NULL);
	
	string strRet(temp);
	
	delete [] temp;
	
	return strRet;
}

string GeneralRandomString( int len )
{
	static unsigned int seed = 0;
	
	if (seed == 0){
		seed = GetTickCount();
		srand(seed);
	}

	char*	temp = new char[len+1];

	for(int i=0; i!=len; i++)
	{
		char chPswChar = 0;
		while (1)
		{
			chPswChar = (char)rand()%122;
			if (((chPswChar >=48)&&(chPswChar<=57))||((chPswChar>=65)&&(chPswChar<=90))||((chPswChar>=97)&&(chPswChar<=122))){
				temp[i] = chPswChar;
				break;
			}
		}
	}

	temp[len] = 0;

	string strRet = temp;
	
	delete [] temp;

	return strRet;
}

string GeneralRandomString( int minlen,int maxlen ){

	static unsigned int seed2 = 0;
	if (seed2 == 0)
	{
		seed2 = GetTickCount();
		srand(seed2);
	}

	if (minlen >= maxlen){
		return GeneralRandomString(maxlen);
	}
	
	int maxsubmin = maxlen - minlen + 1;

	char	*temp = new char[maxlen+1];
	memset(temp,0,maxlen+1);

	for(int i=0; i!=maxlen; i++){

		char chPswChar = 0;
		
		while (1)
		{
			chPswChar = (char)rand()%122;
			if (((chPswChar >=48)&&(chPswChar<=57))||((chPswChar>=65)&&(chPswChar<=90))||((chPswChar>=97)&&(chPswChar<=122))){
				temp[i] = chPswChar;
				break;
			}
		}
	}
	
	temp[maxlen] = 0;
	string str = temp;

	str = str.substr(0,minlen+(rand()%(maxlen-minlen+1)));

	delete [] temp;

	return str;
}

