#include "stdafx.h"
#include "Paths.h"
#include <string>
using std::string;
string getAppDir(){
	char moduleName[MAX_PATH];
	GetModuleFileNameA(NULL, moduleName, sizeof(moduleName));
	string strAppDir(moduleName);
	string::size_type pos = strAppDir.rfind('\\');

	if (pos == string::npos || pos == strAppDir.length() - 1) {
		return strAppDir;
	} else {
		strAppDir = strAppDir.substr(0, pos);
		return strAppDir;
	}
}
