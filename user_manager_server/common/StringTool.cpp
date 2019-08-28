#include "stdafx.h"
#include "StringTool.h"

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