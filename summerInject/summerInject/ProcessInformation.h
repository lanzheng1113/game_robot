#pragma once

#include <string>
using std::string;

class ProcessInformation
{
public:
	ProcessInformation(void){
		_PID = 0;
		_exeFileName = "unknow process";
		_threadCount = 0;
	}

	ProcessInformation(const ProcessInformation& pi){
		*this = pi;
	}

	~ProcessInformation(void){}
	
	friend bool operator < (const ProcessInformation& pi1,const ProcessInformation& pi2);
	friend bool operator == (const ProcessInformation& pi1,const ProcessInformation& pi2);

	inline ProcessInformation& operator = (const ProcessInformation& pi){
		_PID = pi._PID;
		_exeFileName = pi._exeFileName;
		_threadCount = pi._threadCount;
		return *this;
	}

	unsigned int PID() const { return _PID; }
	void setPID(unsigned int val) { _PID = val; }
	string ExeFileName() const { return _exeFileName; }
	void setExeFileName(string val) { _exeFileName = val; }
	unsigned int ThreadCount() const { return _threadCount; }
	void setThreadCount(unsigned int val) { _threadCount = val; }

private:
	unsigned int _PID;
	string _exeFileName;
	unsigned int _threadCount;

};

inline bool operator < (const ProcessInformation& pi1,const ProcessInformation& pi2){
	return pi1._PID < pi2._PID;
}

inline bool operator > (const ProcessInformation& pi1,const ProcessInformation& pi2){
	return !(pi1 < pi2);
}

inline bool operator == (const ProcessInformation& pi1,const ProcessInformation& pi2){
	return (pi1._PID == pi2._PID && pi1._exeFileName == pi2._exeFileName && pi1._threadCount == pi2._threadCount);
}

inline bool operator != (const ProcessInformation& pi1,const ProcessInformation& pi2){
	return !(pi1 == pi2);
}