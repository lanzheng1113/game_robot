#pragma once
/**
* @file NetworkCallBack.h ����NetworkCallBack
* @brief NetworkCallBack�߲��io�ӿ�
*
* Class NetworkCallBack����ص����Ƕ�����IO�ķ�װ������ÿһ�����Ӷ��ж�Ӧ��һ��NetworkCallBackʵ������ʵ�����ڴ���send���̺ͽ���recv֪ͨ��
* @author Vincent
* @date 2012/7/20
*/
class session;

/**
* @brief NetworkCallBack����߲��io�ӿ�.
*
* NetworkCallBack�ṩ��һ�����Ľӿڹ�ҵ���߼�ʹ�á�ʹ��NetworkCallBack���õķ�װ�����Լ�����ȫ���ӵײ�ʵ�֡�
* ʹ��NetworkCallBack�Ľӿڿ��Է���дҵ����صĴ��룬�����ÿ���������send/recv/diconnect���̡�
* <B>ע�������Ҫ��send/recv�����ض���������⴦����Ҫ������Ӵ��롣</B>
*/
class NetworkCallBack
{
public:
	/**
	* Ĭ�Ϲ��캯��
	*/
	NetworkCallBack(void);
	/**
	* NetworkCallBack��������Ϊ������ƣ������������������virtual�ġ�
	*/
	virtual ~NetworkCallBack(void);
public:
	/**
	* ��ȡ�����������Ӧ�ĵײ�session����connection����
	*/
	session *	GetSession() { return m_pSession; }
	/**
	* ������������Ӧ�ĵײ�session����
	*/
	VOID	SetSession( session *pSession ) { m_pSession = pSession; }
	/**
	* �������peer��IP
	*/
	char*	GetIP();
public:
	/**
	* �麯�����ص�����
	* ���ײ�session�ر�ʱ�����á�
	*/
	virtual VOID	OnDisconnect() = 0;
	/**
	* �麯�����ص�����
	* ��session�յ�һ�����������ݰ�ʱ�����á�
	* @param pMsg:���ݰ����ݣ������������ֽڵ�����ͷ��������ͷ�ڵ�������ص�ʱ�Ѿ����ų���
	* @param wSize:���ݰ��ĳ��ȡ�
	*/
	virtual	VOID	OnRecv( BYTE *pMsg, WORD wSize ) = 0;
	/**
	* �麯�����ص�����
	* �ײ��¼�¼�ʱ���á�
	* @param pszLog:�¼�����
	*/
	virtual VOID	OnLogString( char *pszLog ) {}
	/**
	* �����緢��һ�����ݰ�
	* @param pMsg:���ݰ�
	* @param wSize:���ݰ�����
	*/
	BOOL			Send( BYTE *pMsg, WORD wSize );
	/**
	* �����緢�Ͷ�����ݶΣ���Щ���ݶ������һ�����ݰ�
	* @param dwNumberOfMessages:���ݰ�����
	* @param ppMsg:��������
	* @param pwSize:���ݳ������飬�����������Ӧ��
	*/
	BOOL			SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	/**
	* �����Ͽ������������,��������Ͽ������յ�OnDisconnect()��Ϣ��
	*/
	VOID			Disconnect();
private:
	session			*m_pSession;

};
