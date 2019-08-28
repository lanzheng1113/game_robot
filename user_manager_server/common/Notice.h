#pragma once
#include <string>
#include "util/Singleton.h"

class CNotice : public Singleton<CNotice>
{
public:
	CNotice(void);
	~CNotice(void);
	void setNoticeFileName(const std::string& filename);
	bool read();
	std::string getContent(){ return _content; }
	unsigned long getCrc(){ return _crc; }
private:
	std::string _content;
	std::string _filename;
	unsigned long _crc;
};
