#include "stdafx.h"
#include "MyUtil.h"
#include "MyNineXUsr.h "
#include <fstream>
#include "util/path.h"
#include "util/File.h"
#include "util/datetime.h"
void LogKaiKaResult( std::vector<CMyNineXUsr*> vecResult )
{
	if (vecResult.empty()){
		return;
	}

	std::string logFilePath = Path::getApplicationDirPath();
	logFilePath += "开卡记录.txt";
	
	std::ofstream ofs;
	if (File::exists(logFilePath))
	{
		ofs.open(logFilePath.c_str(),std::ios_base::app);
	}else{
		ofs.open(logFilePath.c_str());
	}
	if (!ofs.is_open()){
		//MessageBox(NULL,"打开失败了",NULL,MB_OK);
		return;
	}

	ofs << "###开卡记录开始###\n";
	ofs << "##DateTime=" << DateTime().toString().c_str() << "##\n";
	ofs << "##Name=" << vecResult[0]->getName().c_str() << "##\n"; 
	ofs << "##Days=" << vecResult[0]->getCardDaysType() << "##\n";
	ofs << "##Count=" << vecResult.size() << "##\n";

	for (int i=0; i!=vecResult.size(); i++){
		ofs << "#AuthId = "<<vecResult[i]->getAuthId() << "#\n"; 
	}
	ofs << "###开卡记录结束###\n";

	ofs.close();
	return;
}