#pragma once

#include <string>
using std::string;

void StringTrimBlankW(wchar_t* str);
CString StringToCString(const string& str);
string CStringToString(const CString& str);

string GeneralRandomString(int len);
string GeneralRandomString(int minlen,int maxlen);