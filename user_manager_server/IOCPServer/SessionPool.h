#ifndef _SESSIONPOOL_H_
#define _SESSIONPOOL_H_
/**
* @file SessionPool.h
* @author Vincent
* @date 7/27/2012
*
* @brief SessionPool �Ự����Ԥ����ĻỰ��Դ�б�
*
* ��һ���Ự����ʱ���ӻỰ��������Ự��Դ����һ���Ự��ע��ʱ���Ự�ػ��ա�<B>�򵥵�˵���ǡ����ڴ濪����CPU������</B>
*/
#include <windows.h>
#include <vector>

using namespace std;

class session;
class SessionList;

/**
* @brief  SessionPool �Ự����Ԥ����ĻỰ��Դ�б�
*/
class SessionPool
{
public:
	/**
	* ���캯���������Ự�б�
	* @param dwSize �б��ܴ�С
	* @param dwSendBufferSize ���ͻ���������
	* @param dwRecvBufferSize ���ջ���������
	* @param dwMaxPacketSize ���ݰ�������С
	* @param dwTimeOutTick session�ĳ�ʱʱ��
	* @param dwIndexStart ��һ��session��id,����sessionʱid������
	* @remark FIXME:ʵ���ϲ�δ�õ�session��id.
	*/
	SessionPool( DWORD dwSize, DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOutTick, DWORD dwIndexStart );
	/**
	* ��������
	*/
	~SessionPool();
	/**
	* ��SessionPool������һ��session.
	* ��Ϊ��Ԥ�ȷ�����ڴ���Դ�ģ�����ʮ��Ѹ�١�ֻ�Ǽ򵥴ӵ�ǰ��session����һ����
	* @return ���ؿ����õ�sessionָ�룬���û�п��õ�session�򷵻�NULL.
	*/
	session*		Alloc();
	/**
	* ����һ��session��session Pool.
	* @param pSession ָ��session��ָ��
	* @remark <B>FIXME:û�м��pSession����Ч��</B>
	*/
	void			Free( session* pSession );
	/**
	* ���session list�ĳ��ȡ�
	* @return ���ص�ǰ����session�б�ĳ���
	*/
	int				GetLength();

private:
	VOID			Create( DWORD dwSize, DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwTimeOutTick, DWORD dwMaxPacketSize, DWORD dwIndexStart );

	DWORD			m_dwMaxSize;
	SessionList		*m_pList;
};

#endif
