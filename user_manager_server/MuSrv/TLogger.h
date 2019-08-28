#pragma once
#include "util/Singleton.h"
class CTLogger : public Singleton<CTLogger>
{
public:
	CTLogger(void);
	~CTLogger(void);
public:
	bool init();
	void log(const char* format, ...);
	void close();
};

#ifndef TLOG
#define TLOG CTLogger::getInstance().log
#endif