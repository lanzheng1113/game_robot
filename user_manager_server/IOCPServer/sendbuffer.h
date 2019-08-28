#ifndef _SENDBUFFER_H
#define _SENDBUFFER_H
/**
* @file recvbuffer.h ���巢�ͻ�����SendBuffer
* @brief ���ͻ���������WSASend���̡�
* @author: Vincent.
*/
#include <windows.h>
#include "../COMMON/CircuitQueue.h"
#include "../COMMON/NetBase.h"

/**
* @brief ���ͻ���������ӿ���send�Ķ�����Ӧ��
*
* ����ջ�������Completion�����������ݶ�Ӧ�ģ�sendBuffer��Completion������Ϊ������ݡ�
* ��send�����󱻴�����ϣ������Completion�����������ݡ�
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
	* ������������������ʱ������һ�Ρ���������ʱ���ᱻ���á�
	*/
	inline VOID Create( int nBufferSize, DWORD dwExtraBuffeSize ) {
		if( m_pQueue ) delete m_pQueue;
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBuffeSize );
		m_bComplete = TRUE; }
	/**
	* ����������ڵ��������ݣ���ʼ���ͻ�������ʱ���á�
	*/
	inline VOID Clear() { m_pQueue->Clear(); m_bComplete = TRUE; }

	/**
	* sendbuffer��send�������ʱ�����õ�������̡�ִ����˺�����������������,�ο�GetSendParam˵��
	* @param nBytesSend ��ɷ����ֽ�����
	* @remark �ӻ�������ɾ���Ѿ����͵����ݣ�ʵ���Ͻ����ǽ����ȸı䡣
	*/
	inline VOID Completion( int nBytesSend ) { 	m_pQueue->Dequeue( NULL, nBytesSend ); m_bComplete = TRUE; }

	/**
	* ���sendbuffer�Ļ������������ڷ������ݣ�����ָ��ǰ����β����ָ��͵�ǰ���г��ȡ�
	* ע����ִ����˺����󣬽���������������ֱ������������ִ��Completion������Ž�������Completion������˵����
	* @param ppSendPtr ָ�򻺳���ͷָ���ָ��
	* @param nLength [in]���س���
	* @return �ɹ�����TRUE���򷵻�FALSE.
	*/
	inline BOOL	GetSendParam( BYTE **ppSendPtr, int &nLength ) {
		if( !IsReadyToSend() ) { nLength = 0; return FALSE; }
		*ppSendPtr	= m_pQueue->GetReadPtr();
		nLength		= m_pQueue->GetReadableLen();
		m_bComplete = FALSE;
		return TRUE; }
	/**
	* д�����ݵ����绺���������罫�������ʵ�ʱ�����ݷ��ͳ�ȥ��
	* @param pHeader ���ݰ�ͷ�����͵�����2���ֽڵ�WORD���� @see tagPACKET_HEADER
	* @param pMsg ���ݰ���
	* @return ������ݳ��ȳ���������������д������ʧ�ܷ���FALSE���򷵻�TRUE.
	*/
	inline BOOL Write( tagPACKET_HEADER *pHeader, BYTE *pMsg )	{
		if( !m_pQueue->Enqueue( (BYTE*)pHeader, sizeof(PACKET_HEADER) ) )	return FALSE;
		if( !m_pQueue->Enqueue( pMsg, pHeader->size ) )						return FALSE;
		return TRUE; }
	/**
	* д�����ݵ����绺����������������û��ָ������ͷ�������ݰ�������ͷ�ɲ���wSize����
	* @param pMsg ���ݰ������ݰ���
	* @param wSize ���ݰ��ĳ��ȣ��ó���ֵ����Ϊ����ͷ�����ͳ�ȥ����
	* @return �����ݳ��ȳ��ѻ����������򷵻�FALSE,���򷵻�TRUE.
	*/
	inline BOOL Write( BYTE *pMsg, WORD wSize ) {
		if( !m_pQueue->Enqueue( pMsg, wSize ) ) return FALSE;
		return TRUE; }

	/**
	* ��û������ĵ�ǰ���ݳ���
	* @return ��ǰ���ݳ��ȡ�
	*/
	inline DWORD GetLength() { return m_pQueue->GetLength(); }

private:
	BOOL			IsReadyToSend() { return ( m_bComplete && m_pQueue->GetLength() > 0 ) ? TRUE : FALSE; }//<!�������Ƿ�׼���÷������ݣ���PreSendδ���ʱ�������ٴη��͡�

private:
	BOOL				m_bComplete;
	CircuitQueue<BYTE>	*m_pQueue;
};

#endif