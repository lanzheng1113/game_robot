#pragma once
#include "util/Singleton.h"
#include "util/StringList.h"
#include <string>
using std::string;
class InjectHistory : public Singleton<InjectHistory>
{
public:
	InjectHistory();
	~InjectHistory();
public:
	void load();
	void save();
	void add(const string& record);
	void del(const string& record);
	StringList getHistory();
protected:
private:
	StringList _historyList;
};