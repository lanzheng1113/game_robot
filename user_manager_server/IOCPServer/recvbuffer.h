#ifndef _RECVBUFFER_H
#define _RECVBUFFER_H
/**
* @file recvbuffer.h ������ջ�������RecvBuffer
* @author:Vincent
* @brief RecvBuffer����IO���ջ������ķ�װ
*
* �ڱ��汾�У����ջ�������һ�����λ�����CircuitQueue<BYTE>��װʵ�֡�
*/
#include <windows.h>
#include "../common/CircuitQueue.h"
#include "../common/NetBase.h"

/**
* @brief RecvBuffer����IO���ջ������ķ�װ
* �ڱ��汾�У����ջ�������һ�����λ�����CircuitQueue<BYTE>��װʵ�֡�
* ���ṩ����iocp��ɶ˿�ģʽ���Ӧ�Ľӿڣ�����Completion�����ǵ�һ��recv������Ӧʱ�����õ����̡�
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
	* ����һ��RecvBuffer��������
	* @param nBufferSize:����������
	* @param dwExtraBufferSize:���λ��������еĶ��ⳤ�ȡ�
	* @see CircuitQueue
	*/
	inline VOID	Create( int nBufferSize, DWORD dwExtraBufferSize ) {
		if( m_pQueue ) delete m_pQueue;
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBufferSize ); }
	/**
	* ���ջ��������д�����
	* ������ѹ�����ָ�����ȵ��ֽ���
	* @param nBytesRecvd д�볤��
	* @see WSARecv
	*/
	inline VOID	Completion( int nBytesRecvd ) { m_pQueue->Enqueue( NULL, nBytesRecvd ); }
	/**
	* ��ջ���������������Ϊ0.�ڻ�������ʼ��ʱ�����߻�������ʱ��Ҫ��ջ�������
	*/
	inline VOID	Clear() { m_pQueue->Clear(); }
	/**
	* ��ý��ջ������Ŀ�ʼָ��ͳ��ȡ�
	* @param ppRecvPtr ָ�򻺳�����ʼ��ַָ���ָ��
	* @param nLength �������á�<B>FIXME:��Ϊָ���Ϊ���ʣ���ΪnLengthû���κ��������塣</B>
	* @remark ��Ͷ��һ��recv����iocp�˿�ʱ����Ҫ�ṩָ����ջ������Ŀ�������ָ��ͳ�����Ϊ������@see WSARecv
	*/
	inline VOID	GetRecvParam( BYTE **ppRecvPtr, int &nLength ) {
		*ppRecvPtr	= m_pQueue->GetWritePtr();		
		nLength		= m_pQueue->GetWritableLen(); }
	/**
	* ����յ��ĵ�һ�����ݰ��������ݳ���δ��һ�ֽ��򷵻�NULL.
	* @return ָ���һ�����ݰ��Ŀ�ʼλ�á�
	* @remark ������Ϻ󣬴�����ջ������յ������ݰ���
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
	* �ӻ�������ɾ����һ�����ݰ�
	* @param wSize:���ݰ��ĳ��ȡ�
	* @remark:��������һ�����ݰ�ʱ���ô˺�����λ�����ָ��ָ����һ�����ݰ���
	*/
	inline VOID	RemoveFirstPacket( WORD wSize ) { m_pQueue->Dequeue( NULL, wSize ); }

private:
	CircuitQueue<BYTE>	*m_pQueue;
};

#endif