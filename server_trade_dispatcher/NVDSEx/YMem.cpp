#include "StdAfx.h"
#include "YMem.h"
#include "../common/AutoLock.h"
#include "../common/DebugMessage.h"
#include <map>

using std::map;
using std::make_pair;

std::map<int,CYMem> _mem_pool;
static CLock_CS  y_cs;

void* YAlloc(SIZE_T size,const string& type_name){
	CAutoLock lock(y_cs);
	CYMem me;
	me._mem_pointer = malloc(size);
	me._size = size;
	me._type_name = type_name;

	int address = (int)me._mem_pointer;
	_mem_pool.insert(make_pair(address,me));
	return me._mem_pointer;
}


void YFree(void* _mem_pointer){
	CAutoLock lock(y_cs);
	int address = (int)_mem_pointer;
	_mem_pool.erase(address);
	free(_mem_pointer);
}

void YMemDump(){
	CAutoLock lock(y_cs);
	for (std::map<int,CYMem>::const_iterator it = _mem_pool.begin(); it != _mem_pool.end(); ++it)
	{
		char desc[400];
		sprintf_s(desc,sizeof(desc),"block:0x%08x.size=%d,name=%s",it->first,it->second._size,it->second._type_name.c_str());
		DebugMessageA("%s",desc);
	}
}