#ifndef _SENDBUFFER_H
#define _SENDBUFFER_H
/**
* @file recvbuffer.h 定义发送缓冲区SendBuffer
* @brief 发送缓冲区用于WSASend过程。
* @author: Vincent.
*/
#include <windows.h>
#include "../COMMON/CircuitQueue.h"
#include "../COMMON/NetBase.h"

/**
* @brief 发送缓冲区，其接口与send的动作对应。
*
* 与接收缓冲区的Completion函数输入数据对应的，sendBuffer的Completion函数则为输出数据。
* 当send的请求被处理完毕，则调用Completion函数清理数据。
*/
class SendBuffer
{
public:
	/**
	* default.
	*/
	SendBuffer()			{ m_pQueue = NULL; }
	/**
	* default
	*/
	virtual ~SendBuffer()	{ if( m_pQueue ) delete m_pQueue; }
	/**
	* 创建缓冲区，仅创建时被调用一次。回收利用时不会被调用。
	*/
	inline VOID Create( int nBufferSize, DWORD dwExtraBuffeSize ) {
		if( m_pQueue ) delete m_pQueue;
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBuffeSize );
		m_bComplete = TRUE; }
	/**
	* 清除缓冲区内的所有数据，初始化和回收利用时调用。
	*/
	inline VOID Clear() { m_pQueue->Clear(); m_bComplete = TRUE; }

	/**
	* sendbuffer在send请求完成时被调用的完成例程。执行完此函数本缓冲区被解锁,参考GetSendParam说明
	* @param nBytesSend 完成发送字节数。
	* @remark 从缓冲区中删除已经发送的数据，实现上仅仅是将长度改变。
	*/
	inline VOID Completion( int nBytesSend ) { 	m_pQueue->Dequeue( NULL, nBytesSend ); m_bComplete = TRUE; }

	/**
	* 获得sendbuffer的缓冲区参数用于发送数据，包括指向当前数据尾部的指针和当前空闲长度。
	* 注意在执行完此函数后，将锁定本缓冲区。直到本缓冲区被执行Completion函数后才解锁，见Completion函数的说明。
	* @param ppSendPtr 指向缓冲区头指针的指针
	* @param nLength [in]返回长度
	* @return 成功返回TRUE否则返回FALSE.
	*/
	inline BOOL	GetSendParam( BYTE **ppSendPtr, int &nLength ) {
		if( !IsReadyToSend() ) { nLength = 0; return FALSE; }
		*ppSendPtr	= m_pQueue->GetReadPtr();
		nLength		= m_pQueue->GetReadableLen();
		m_bComplete = FALSE;
		return TRUE; }
	/**
	* 写入数据到网络缓冲区，网络将在它合适的时候将数据发送出去。
	* @param pHeader 数据包头，典型的它是2个字节的WORD类型 @see tagPACKET_HEADER
	* @param pMsg 数据包体
	* @return 如果数据长度超过缓冲区长度则写入数据失败返回FALSE否则返回TRUE.
	*/
	inline BOOL Write( tagPACKET_HEADER *pHeader, BYTE *pMsg )	{
		if( !m_pQueue->Enqueue( (BYTE*)pHeader, sizeof(PACKET_HEADER) ) )	return FALSE;
		if( !m_pQueue->Enqueue( pMsg, pHeader->size ) )						return FALSE;
		return TRUE; }
	/**
	* 写入数据到网络缓冲区，此重载类型没有指定数据头。该数据包的数据头由参数wSize决定
	* @param pMsg 数据包的数据包体
	* @param wSize 数据包的长度（该长度值将作为数据头被发送出去）。
	* @return 如数据长度超脱缓冲区长度则返回FALSE,否则返回TRUE.
	*/
	inline BOOL Write( BYTE *pMsg, WORD wSize ) {
		if( !m_pQueue->Enqueue( pMsg, wSize ) ) return FALSE;
		return TRUE; }

	/**
	* 获得缓冲区的当前数据长度
	* @return 当前数据长度。
	*/
	inline DWORD GetLength() { return m_pQueue->GetLength(); }

private:
	BOOL			IsReadyToSend() { return ( m_bComplete && m_pQueue->GetLength() > 0 ) ? TRUE : FALSE; }//<!缓冲区是否准备好发送数据，当PreSend未完成时不允许被再次发送。

private:
	BOOL				m_bComplete;
	CircuitQueue<BYTE>	*m_pQueue;
};

#endif