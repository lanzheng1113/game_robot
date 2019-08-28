#include "util/singleton.h"

class OffsetMgr : public Singleton<OffsetMgr>
{
public:
	DWORD genBaiTanOffset(DWORD* pOffset,WORD count=40);
	DWORD genTrueOffset(DWORD* pOffset,WORD count=40);
	DWORD genFalseOffset(DWORD* pOffset,WORD count=40);
	void  reloadOffsetFromInis();
	void  remoteUpdate(DWORD* pOffset,WORD count);
protected:
private:
};
