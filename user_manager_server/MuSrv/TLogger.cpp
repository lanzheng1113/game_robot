#include "StdAfx.h"
#include "TLogger.h"
#include <fstream>
#include "util/Path.h"
#include "util/datetime.h"
#include "util/File.h"
#include "DebugMessage.h"
static std::ofstream fwriter;

CTLogger::CTLogger(void)
{
}

CTLogger::~CTLogger(void)
{
}

void CTLogger::log(const char* format, ...)
{
	if (!fwriter.is_open())
	{
		DebugMessage("必须在使用之前先打开文件流");
		return;
	}

	char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	fwriter << buffer << std::endl;
}

bool CTLogger::init()
{
	if (fwriter.is_open())
	{
		return true;
	}

	std::string logPath = Path::getApplicationDirPath();
	logPath += "ctLog.txt";
	
	if (!File::exists(logPath))
	{
		fwriter.open(logPath.c_str());
	}else
		fwriter.open(logPath.c_str(),std::ios::app);

	if (!fwriter.is_open())
	{
		return false;
	}
	else
	{
		fwriter << std::endl;
		fwriter << "--------------------------------------------------" << std::endl;
		fwriter << "Logger start at " << DateTime().toString() << std::endl;
		fwriter << "--------------------------------------------------" << std::endl;
		return true;
	}
}

void CTLogger::close()
{
	if (fwriter.is_open())
	{
		fwriter.close();
	}
}
