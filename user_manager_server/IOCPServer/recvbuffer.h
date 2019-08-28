#ifndef _RECVBUFFER_H
#define _RECVBUFFER_H
/**
* @file recvbuffer.h 定义接收缓冲区类RecvBuffer
* @author:Vincent
* @brief RecvBuffer类是IO接收缓冲区的封装
*
* 在本版本中，接收缓冲区由一个环形缓冲区CircuitQueue<BYTE>封装实现。
*/
#include <windows.h>
#include "../common/CircuitQueue.h"
#include "../common/NetBase.h"

/**
* @brief RecvBuffer类是IO接收缓冲区的封装
* 在本版本中，接收缓冲区由一个环形缓冲区CircuitQueue<BYTE>封装实现。
* 它提供了与iocp完成端口模式想对应的接口，例如Completion例程是当一个recv请求被响应时被调用的例程。
* 
*/
class RecvBuffer
{
public:
	/**
	* default construct.
	*/
	RecvBuffer()			{ m_pQueue = NULL; }
	/**
	* default des.
	*/
	virtual ~RecvBuffer()	{ if( m_pQueue ) delete m_pQueue; }
	/**
	* 创建一个RecvBuffer缓冲区。
	* @param nBufferSize:缓冲区长度
	* @param dwExtraBufferSize:环形缓冲区队列的额外长度。
	* @see CircuitQueue
	*/
	inline VOID	Create( int nBufferSize, DWORD dwExtraBufferSize ) {
		if( m_pQueue ) delete m_pQueue;
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBufferSize ); }
	/**
	* 接收缓冲区完成写入过程
	* 缓冲区压入参数指定长度的字节数
	* @param nBytesRecvd 写入长度
	* @see WSARecv
	*/
	inline VOID	Completion( int nBytesRecvd ) { m_pQueue->Enqueue( NULL, nBytesRecvd ); }
	/**
	* 清空缓冲区，长度设置为0.在缓冲区初始化时，或者回收利用时需要清空缓冲区。
	*/
	inline VOID	Clear() { m_pQueue->Clear(); }
	/**
	* 获得接收缓冲区的开始指针和长度。
	* @param ppRecvPtr 指向缓冲区开始地址指针的指针
	* @param nLength 长度引用。<B>FIXME:改为指针更为合适，因为nLength没有任何输入意义。</B>
	* @remark 当投递一个recv请求到iocp端口时，需要提供指向接收缓冲区的空闲区块指针和长度作为参数。@see WSARecv
	*/
	inline VOID	GetRecvParam( BYTE **ppRecvPtr, int &nLength ) {
		*ppRecvPtr	= m_pQueue->GetWritePtr();		
		nLength		= m_pQueue->GetWritableLen(); }
	/**
	* 获得收到的第一个数据包，如数据长度未满一字节则返回NULL.
	* @return 指向第一个数据包的开始位置。
	* @remark 接收完毕后，处理接收缓冲区收到的数据包。
	*/
	inline BYTE* GetFirstPacketPtr() {
		PACKET_HEADER   header;
		// 
		if( !m_pQueue->Peek( (BYTE*)&header, sizeof(PACKET_HEADER) ) )				return NULL;
		// 
		if( m_pQueue->GetLength() < (int)( sizeof(PACKET_HEADER) + header.size ) )	return NULL;
		// 
		if( m_pQueue->GetBackDataCount() < (int)( header.size + sizeof(header) ) ) {
			m_pQueue->CopyHeadDataToExtraBuffer( header.size + sizeof(header) - m_pQueue->GetBackDataCount() ); }
		return  m_pQueue->GetReadPtr();	}
	/**
	* 从缓冲区中删除第一个数据包
	* @param wSize:数据包的长度。
	* @remark:当处理完一个数据包时调用此函数令环形缓冲区指针指向下一个数据包。
	*/
	inline VOID	RemoveFirstPacket( WORD wSize ) { m_pQueue->Dequeue( NULL, wSize ); }

private:
	CircuitQueue<BYTE>	*m_pQueue;
};

#endif