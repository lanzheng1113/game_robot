#include "StdAfx.h"
#include "OrderRecorder.h"
#include "../common/DebugMessage.h"
#include "../common/AutoLock.h"
#include "util/Logger.h"
#include "util/DateTime.h"
using std::ios_base;
using std::endl;

COrderRecorder::COrderRecorder(const string& recorder_name)
{
	_is_open = false;
	_recorder_name = recorder_name;
	_log_lock = new CLock_CS();
}

COrderRecorder::~COrderRecorder(void)
{
	if (_is_open){
		//do sth to close it.
		_file.close();
	}
	
	if (_log_lock)
	{
		delete _log_lock;
		_log_lock = NULL;
	}

}

void COrderRecorder::Log( const string& informations )
{
	CAutoLock locker(*_log_lock);

	if (!_is_open){
		//do sth to open it.
		_file.open(_recorder_name.c_str(),ios_base::out | ios_base::app);
		if (!_file.is_open()){
			//DebugMessageA("open file %s failed with last error %d!",_recorder_name.c_str(),GetLastError());
			LOG_ERROR("open file %s failed with last error %d!",_recorder_name.c_str(),GetLastError());
			return;
		}
		_is_open = true;
	}
	
	ASSERT(_is_open);
	string strtime = DateTime().toString();
	strtime += "      ";
	_file << strtime + informations << endl;

//	DebugMessageA("%s",informations.c_str());
}
