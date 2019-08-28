#include "stdafx.h"
#include "CircuitBuffer.h"

CCircleByteBuffer::CCircleByteBuffer(int nSize)
: m_nSize(nSize)
, m_nRead(0)
, m_nWrite(m_nRead)
{
	m_pBuf = new char [m_nSize];
}

CCircleByteBuffer::~CCircleByteBuffer()
{
	delete [] m_pBuf;
}

int CCircleByteBuffer::push(const char * pData, int len)
{
	CAutoLock lock(m_cs);

	if (len <= 0)
		return 0;

	int free_len = FreeLength() - 1;

	if (free_len <= 0)
		return 0;

	int nData = (free_len >= len) ? len : free_len;

	if (m_nRead > m_nWrite) {
		memcpy(m_pBuf + m_nWrite, pData, nData);
		m_nWrite += nData;
	}
	else {
		int tail_len = m_nSize - m_nWrite;
		tail_len = (tail_len >= nData) ? nData : tail_len;
		memcpy(m_pBuf + m_nWrite, pData, tail_len);
		m_nWrite = (m_nWrite + tail_len) % m_nSize;
		memcpy(m_pBuf, pData + tail_len, nData - tail_len);
	}
	return nData;
}

int CCircleByteBuffer::pop(char * pData, int len)
{
	CAutoLock lock(m_cs);

	if (len <= 0)
		return 0;

	int data_len = DataLength();

	if (data_len <= 0)
		return 0;

	int nData = (data_len >= len) ? len : data_len;

	if (m_nWrite > m_nRead) {
		memcpy(pData, m_pBuf + m_nRead, nData);
		m_nRead += nData;
	}
	else {
		int tail_len = m_nSize - m_nRead;
		tail_len = (tail_len >= nData) ? nData : tail_len;

		memcpy(pData, m_pBuf + m_nRead, tail_len);
		m_nRead = (m_nRead + tail_len) % m_nSize;
		memcpy(pData + tail_len, m_pBuf, nData - tail_len);
	}
	return nData;
}

bool CCircleByteBuffer::IsFull()
{
	CAutoLock lock(m_cs);
	return (FreeLength() == 1);
}

bool CCircleByteBuffer::IsEmpty()
{
	CAutoLock lock(m_cs);
	return (m_nWrite == m_nRead);
}

int CCircleByteBuffer::DataLength() const
{
	int len = m_nWrite - m_nRead;
	if (len < 0)
		len = m_nSize + len;
	return len;
}

int CCircleByteBuffer::FreeLength() const
{
	int len = m_nRead - m_nWrite;
	if (len <= 0)
		len = m_nSize + len;
	return len;
}

int CCircleByteBuffer::getDataLength()
{
	CAutoLock lock(m_cs);
	return DataLength();
}

int CCircleByteBuffer::getFreeLength()
{
	CAutoLock lock(m_cs);
	return FreeLength();
}