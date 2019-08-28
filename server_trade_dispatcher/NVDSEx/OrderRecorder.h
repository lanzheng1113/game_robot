#pragma once

#include <fstream>
#include "util/noncopyable.h"
#include <string>
using std::string;
using std::ofstream;
class CLock_CS;

class COrderRecorder :
	public NonCopyable
{
public:
	COrderRecorder(const string& recorder_name);
	~COrderRecorder(void);
public:
	void Log(const string& informations);
private:
	string _recorder_name;
	string _str_logs;
	bool _is_open;
	ofstream _file;
	CLock_CS * _log_lock;
};
