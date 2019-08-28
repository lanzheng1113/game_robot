#pragma once
#include <string>
#include <vector>
#include <map>

enum{
	ERROR_OPEN_SCRIPT_FILE_CANNOT_OPEN = -1,
};
struct FileInfo {
	std::string name;
	unsigned int size_file;
	unsigned int crc;
	char* pContent;
};

class FileUpDownLoad
{
public:
	FileUpDownLoad(void);
	~FileUpDownLoad(void);
public:
	int loadFiles();
	int calcNeedSize(std::map<std::string,unsigned int>& clientFiles);
	void fillPostContents( BYTE* m_extraSendBuffer,std::map<std::string,unsigned int>& clientFiles );
private:
	int openfile(const std::string& name,FileInfo& fileinfo);
	std::vector<FileInfo> _fi_vec;
};
