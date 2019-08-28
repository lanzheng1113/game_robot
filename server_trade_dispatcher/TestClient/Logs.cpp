#include "StdAfx.h"
#include "Logs.h"
#include <stdarg.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

Logs* Logs::_instance = NULL;

Logs::Logs(void)
{
	_logFileState = LogFileState_Unset;
}

Logs::~Logs(void)
{
	flush();
	_file.close();
}

void Logs::setLogFileName( const string& name )
{
	if (name.empty()) {
		_logFileState = LogFileState_Disabled;
	} else {
		_logFileState = LogFileState_Defined;
		_logFileName = name;
	}
}

void Logs::log( const string& str )
{
	boost::mutex::scoped_lock scopedLock(_mutex);
	if (_logFileState == LogFileState_Unset) {
		std::cerr << "*** Logger::log() called before Logger::setLogFileName() ***" << std::endl;
	} else if (_logFileState == LogFileState_Defined) {
		_file.open(_logFileName.c_str(),std::ios_base::out|std::ios_base::app);
		_file << "#*Section Start!" << std::endl;
		_logFileState = LogFileState_Created;
	}

	if (_logFileState == LogFileState_Created) {
		_file << str << std::endl;
	}

	std::cerr << str << std::endl;
	
}

void Logs::log( const char* format, ... )
{
	char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	log(string(buffer));
}

void Logs::flush()
{
	boost::mutex::scoped_lock scopedLock(_mutex);

	_file.flush();
}

Logs& Logs::getInstance()
{
	if (!_instance){
		_instance = new Logs();
	}
	return *_instance;
}

void C_log( const string& str )
{
	Logs::getInstance().log(str);
}

void C_log( const char* format, ... )
{
	char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	Logs::getInstance().log(string(buffer));
}
