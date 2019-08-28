#include "StdAfx.h"
#include "FileUpDownLoad.h"
#include "util/crc32.h"
#include "util/StringEx.h"
#include "util/Path.h"
#include "util/StringList.h"
#include "util/File.h"
#include "DebugMessage.h"
FileUpDownLoad::FileUpDownLoad(void)
{

}

FileUpDownLoad::~FileUpDownLoad(void)
{

}

int FileUpDownLoad::loadFiles()
{
	//首先清零原有的内容
	for (int i=0; i!=_fi_vec.size(); i++)
	{
		if (_fi_vec[i].pContent)
		{
			delete []  _fi_vec[i].pContent;
		}
	}
	_fi_vec.clear();
	
	//便利目录下的文件，重新生成一份。
	std::string str = Path::getApplicationDirPath();
	str += "Enables\\";

	StringList sl = File(str).getFileListRecursive();
	if (sl.empty()){
		return 0;
	}

	int iLoad = 0;
	for (StringList::const_iterator it = sl.begin(); it!=sl.end(); ++it){
		FileInfo fileinfo;
		int iOpen = openfile(*it,fileinfo);
		bool load_success = false;
		if (iOpen == ERROR_OPEN_SCRIPT_FILE_CANNOT_OPEN)
		{
			return ERROR_OPEN_SCRIPT_FILE_CANNOT_OPEN;
		}else{
			_fi_vec.push_back(fileinfo);
		}
	}

	return _fi_vec.size();
}

int FileUpDownLoad::openfile(const std::string& name,FileInfo& fileinfo )
{
	String s(name);
	String s2 = s.toUpperCase();
	DebugMessage("open file %s",s2.c_str());
	if (!s2.endsWith(".TXT")){
		return ERROR_OPEN_SCRIPT_FILE_CANNOT_OPEN;
	}

	FILE* fp = fopen(name.c_str(),"r");
	if (!fp){
		DebugMessage("打开文件失败了,LastError=%d。",GetLastError());
		return ERROR_OPEN_SCRIPT_FILE_CANNOT_OPEN;
	}else{
		fseek(fp,0,SEEK_END);
		int len = ftell(fp);
		char* _content = new char[len];
		memset(_content,0,len);

		fseek(fp,0,SEEK_SET);
		int read_actully = fread(_content,1,len,fp);
		fclose(fp);

		File ft(name);
		fileinfo.name = ft.getFileName();
		fileinfo.size_file = read_actully;
	 	fileinfo.crc = Crc32::cal_crc((unsigned char*)_content,read_actully);
		DebugMessageA("Srv file name=%s,crc=0x%08X",name.c_str(),fileinfo.crc);
		fileinfo.pContent = _content; //注意没在这里释放内存。

		return read_actully;
	}
}

int FileUpDownLoad::calcNeedSize( std::map<std::string,unsigned int>& clientFiles )
{
	int needlen = 0;
	for (int i=0; i!=_fi_vec.size(); i++)
	{
		FileInfo f = _fi_vec[i];
		if (clientFiles[f.name] != f.crc ){
			DebugMessageA("client files crc = 0x%08X,Srv file crc = 0x%08X",clientFiles[f.name],f.crc);
			//CRC不一样，那么需要传文件过去。
			//[8字节 "TagBegin"]+[32字节 name] + [4字节 Crc32] + [f.len 文件内容]
			needlen += 8;
			needlen += 32;
			needlen += 4;
			needlen += 4;
			needlen += f.size_file;
			DebugMessage("文件NAME=%s,所需长度=%d",f.name.c_str(),f.size_file+44);
		}
	}
	return needlen;
}

void FileUpDownLoad::fillPostContents( BYTE* m_extraSendBuffer,std::map<std::string,unsigned int>& clientFiles )
{
	BYTE* writePos = m_extraSendBuffer;
	for (int i=0; i!=_fi_vec.size(); i++){
		FileInfo f = _fi_vec[i];
		if (clientFiles[f.name] != f.crc ){
			//CRC不一样，那么需要传文件过去。
			//[8字节 "TagBegin"]+[32字节 name] + [4字节 Crc32] + [f.len 文件内容]
			char tag[8] = "TagBegi";
			memcpy(writePos,tag,8);						writePos += 8;
			char name[32] = {0};
			strncpy(name,f.name.c_str(),32); name[31] = 0;
			memcpy(writePos,name,32);					writePos += 32;
			*(DWORD*)writePos = f.crc;					writePos += 4;
			*(DWORD*)writePos = f.size_file;			writePos += 4;
			memcpy(writePos,f.pContent,f.size_file);	writePos += f.size_file;
			DebugMessage("拷贝文件NAME=%s,CRC32=0x%X,所需长度=%d",f.name.c_str(),f.crc,f.size_file+44);
		}
	}
}