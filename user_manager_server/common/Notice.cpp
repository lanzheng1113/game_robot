#include "StdAfx.h"
#include "Notice.h"
#include "util/File.h"
#include "util/Crc32.h"
#include "DebugMessage.h"
#ifdef _SRVIC
#include "util/Logger.h"
#endif
CNotice::CNotice(void)
:_filename(""),
_content(""),
_crc(0)
{
}

CNotice::~CNotice(void)
{
}

void CNotice::setNoticeFileName( const std::string& filename )
{
	_filename = filename;
}

bool CNotice::read()
{
	_content = "";
	_crc = 0;

	if (_filename == "")
	{
#ifdef _SRVIC
		LOG_ERROR("��ͼ��һ������Ϊ�յ���ʾ�ļ�");
#else
		DebugMessage("��ͼ��һ������Ϊ�յ���ʾ�ļ�");
#endif
		return false;
	}
	FileReader FR(_filename);
	if (FR.open())
	{	
		_content = FR.read();
		if (_content.empty()){
			_crc = 0;
		}else
			_crc = Crc32::cal_crc((unsigned char*)_content.c_str(),strlen(_content.c_str()));
		FR.close();
		DebugMessage("file %s\nconcent %s\ncrc %x",_filename.c_str(),_content.c_str(),_crc);
		return true;
	}else{
#ifdef _SRVIC
		LOG_ERROR("���ļ�%sʧ����LastError=%d��",_filename.c_str(),GetLastError());
#else
		DebugMessage("���ļ�%sʧ����LastError=%d��",_filename.c_str(),GetLastError())
#endif
	}
	return false;
}

