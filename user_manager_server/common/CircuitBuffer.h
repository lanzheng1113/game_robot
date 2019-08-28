#ifndef CIRCUITBUFFER_H
#define CIRCUITBUFFER_H
#include "AutoLock.h"
//环形字节缓冲区
class CCircleByteBuffer
{
public:
	CCircleByteBuffer(int nSize);
	virtual~CCircleByteBuffer();
	int push(const char * pData, int len);
	int pop(char * pData, int len);
	bool IsFull();
	bool IsEmpty();
	int getDataLength();
	int getFreeLength();
private:
	int DataLength() const;
	int FreeLength() const;
private:
	CLock_CS m_cs;
	int   m_nSize;
	int   m_nRead;
	int   m_nWrite;
	char *  m_pBuf;
};

#endif //CIRCUITBUFFER_H