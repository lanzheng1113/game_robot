#pragma once
#include <string>
using std::string;
#include <fstream>
using std::ofstream;
#include <boost/thread/mutex.hpp>

class Logs
{
public:
	~Logs(void);
public:
	void setLogFileName(const string& str);
	void log(const string& str);
	void log(const char* format, ...);
	/**
	 * Flushes the logger.
	 */
	 void flush();

	 static Logs& getInstance();
private:
	enum LogFileState {
		LogFileState_Unset,    /** setLogFileName() has not been called yet */
		LogFileState_Disabled, /** setLogFileName() has been called with an empty string */
		LogFileState_Defined,  /** setLogFileName() has been called, but the file has not been created yet */
		LogFileState_Created   /** setLogFileName() has been called and the file has been created */
	};
	LogFileState _logFileState;

	string _logFileName;

	ofstream _file;

	static Logs * _instance;

	Logs(void);
	/** Ensures Logger is thread-safe. */
	mutable boost::mutex _mutex;
};


void C_log(const string& str);
void C_log(const char* format, ...);