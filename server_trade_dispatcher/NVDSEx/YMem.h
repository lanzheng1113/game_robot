#pragma once

#include <string>
using std::string;

class CYMem
{
public:
	void* _mem_pointer;
	size_t _size;
	string _type_name;
};

void* YAlloc(SIZE_T size,const string& type_name);
void YFree(void* _mem_pointer);
void YMemDump();
