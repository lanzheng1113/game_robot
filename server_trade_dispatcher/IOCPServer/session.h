#ifndef _SESSION_H
#define _SESSION_H
/**
* @file session.h �Ự��װ��һ����������
* @author Vincent
* @date 2012-7-26
* @brief ������io������
*
* session����IOCP������ģ���ж��������ӵķ�װ�����session��ioHandle����ִ���շ�������
* ��Ϊ����IO�ĵ��ȵ�Ԫ��ִ��IO��������Ķ���
* ��ͨ��NetworkCallBack���ϴ���Recv֪ͨ����ͨ�����´���Send����ͨ���������send����������
* ͨ��PreSend��PreRecv�������õ�RecvBuffer,SendBufferִ���շ�������
* ����ͨ��session id��ʶΨһ��session��Ҳ����ͨ��session socket��ʶ��
* ����iocp���Լ�iocp�Ľ��ܣ�<a href=http://blog.csdn.net/jifengszf/article/details/1896839>JeffShen��ר��</a>
*/
class SendBuffer;
class RecvBuffer;
class NetworkCallBack;
class session;

#define SEND_POSTED			1///<����io�������ͣ�����
#define RECV_POSTED			2///<����io�������ͣ�����
#define ACCEPT_POSTED		3///<����io�������ͣ���������
#define DISCONNECT_POSTED	4///<����io�������ͣ��Ͽ�����

/**
* ���ڷ���IOCP������첽�����ṹ�塣������ΪOVERLAPPED�������ࡣ
* @see WSASend
* @see WSARecv
*/
struct OVERLAPPEDEX : public OVERLAPPED
{
	DWORD		dwIoSize;	///<io����
	DWORD		dwFlags;	///<ioFlag
	DWORD		dwOperationType;	///<��������SEND_POSTED��RECV_POSTED��ACCEPT_POSTED��DISCONNECT_POSTED֮һ
	session*	pSession;	///<������io�����session
};

/**
* define _cbk_fun ��ʾһ��������һ���ص�������û�����������塣
*/
#ifndef _cbk_fun
#define _cbk_fun
#endif

/**
* @brief session�Ự��װ��һ����������
*
* session����IOCP������ģ���ж��������ӵķ�װ�����session��ioHandle����ִ���շ�������
* ��Ϊ����IO�ĵ��ȵ�Ԫ��ִ��IO��������Ķ���
* ��ͨ��NetworkCallBack���ϴ���Recv֪ͨ����ͨ�����´���Send����ͨ���������send����������
* ͨ��PreSend��PreRecv�������õ�RecvBuffer,SendBufferִ���շ�������
* ����ͨ��session id��ʶΨһ��session��Ҳ����ͨ��session socket��ʶ��
* ����io��ɶ˿ڵľ�����ܣ����Լ�<a href=http://blog.csdn.net/jifengszf/article/details/1896839>JeffShen��ר��</a>
*/
class session
{
public:
	/**
	* ���캯��
	* @param dwSendBufferSize ���ͻ�������С��
	* @param dwRecvBufferSize ���ջ�������С��
	* @param dwMaxPacketSize ���ݰ����������С��
	* @param dwTimeOut ��ʱʱ�䡣
	* FIXME���Ͻ����Ӧ�ý�Ĭ�ϵĹ��캯����Ϊ˽�С�
	*/
	session( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut );
	/**
	* Ĭ����������
	*/
	~session();
protected:
public:
	/**
	* ��ʼ��������ʹ��session����io����֮ǰ����һ�λ��߻�������ʱ���������һ�γ�ʼ��������
	*/
	VOID					Init();

	/**
	* Post a send request to iocp socket.
	* ��ͬ��Send( BYTE *pMsg, WORD wSize )��SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )������д������,PreSend���������ڵ�����д�����硣
	* �������ΪSend��SendEx��һ��Write��������PreSend��һ��refresh������
	* @return �ɹ�����TRUE����ʹ������û������Ҳ����TRUE���������ݷ���������󷵻�FALSE.
	*/
	BOOL					PreSend();
	/**
	* Post a recv request to iocp socket
	* ��io��ɶ˿�Ͷ��һ��RECV���󡣵�������Ӧʱ������RecvBuffer��Completion���д�����ݡ�
	*/
	BOOL					PreRecv();
	/**
	* Set remove flag to remove this from active session list.
	* remove��Ǿ���һ���Ự�Ƿ�Ӧ�ñ��ӻ�Ự�б���ɾ����
	*/
	VOID					Remove() { InterlockedExchange( (long*)&m_bRemove, TRUE ); }
	/**
	* Write data to send buffer.(maybe it need a better function name.)
	* FIXME����Ҫһ�����õ����֡�ʵ����PreSend����������ִ��send����!
	*/
	BOOL					Send( BYTE *pMsg, WORD wSize );	
	/**
	* Write data to send buffer.(maybe it need a better function name.)
	* FIXME����Ҫһ�����õ����֡�ʵ����PreSend����������ִ��send����!
	*/
	BOOL					SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );

	VOID					BindNetworkCallBack(NetworkCallBack* cbk);///<bind network callback to transmit io result.
	VOID					UnbindNetworkCallBack();///<unbind network callback

	/**
	* �ص����������ڴ���OnReceive�����ִ�к��������������һ�����ݰ����ȡ������NetworkCallBack���ݴ���OnReceive�����û���򷵻�TRUE��ִ���κβ�����������FALSE.
	* FIXME:���Խ�dwMaxPacketSize��Ϊ�����Ա�Լ򻯺�����dwMaxPacketSize���ǵ��ڹ���sessionʱ�����dwRecvBufferSize��
	* @param dwMaxPacketSize �������ݰ����������С��
	* @return ִ�гɹ�����TRUE��������ջ�����û������Ҳ����TRUE��ִ�й�����������ʹ����򷵻�FALSE.
	*/
	BOOL					_cbk_fun OnRecv(DWORD dwMaxPacketSize);
public:
	/**
	* inline function for access private members.it's faster than common member function.
	* FIXME:pay attention to these functions.may some functions shoule be removed!
	*/
	NetworkCallBack*		GetNetworkCallBack(){return m_NetworkCallBack;}

	SOCKET					GetSocket()	const	{ return m_socket; }					///<Access socket handle.
	VOID					SetSocket( SOCKET socket ) { m_socket = socket; }			///<Access socket handle.

	SOCKADDR*				GetSockAddr() const	{ return (SOCKADDR*)&m_sockaddr; }		///<Access socket SOCKADDR.
	VOID					SetSockAddr( SOCKADDR_IN& sockaddr ) { m_sockaddr = sockaddr; }///<Access socket SOCKADDR.
	char*					GetIP() const { return inet_ntoa( m_sockaddr.sin_addr ); }	///<Access remote client IP
	
	unsigned short			GetPort() const { return m_usPort; }///<Access remote client port
	void					SetPort(unsigned short val) { m_usPort = val; }///<Access remote client port

	DWORD					GetIndex() const { return m_dwIndex; }///<Access session index,it is unique in all session list.
	void					SetIndex(DWORD val) { m_dwIndex = val; }///<Access session index,it is unique in all session list.

	RecvBuffer*				GetRecvBuffer() const { return m_pRecvBuffer; }///<Access receive buffer @see RecvBuffer
	SendBuffer*				GetSendBuffer() const { return m_pSendBuffer; }///<Access send buffer @see SendBuffer

	VOID					ResetKillFlag() { InterlockedExchange( (long *)&m_bRemove, FALSE ); }///< Reset remove flag.if m_bRemove is TRUE,it will be removed in active session list.
	BOOL					ShouldBeRemoved() { return m_bRemove; }///<Access remove flag.

	/**
	* Call this function to disconnect to remote client.
	*/
	VOID					Disconnect();

	/**
	* Check session is on idle.if on idle.it will be kicked out in active list.
	*/
	BOOL					IsOnIdle(){ return m_dwTimeOut ? (m_dwLastSyncTick + m_dwTimeOut < GetTickCount()) : FALSE; }
	//DWORD					GetTimeOut() const { return m_dwTimeOut; }//timeout tick.set this to kick the session which do not send/recv any packet after connection built.
	//void					SetTimeOut(DWORD val) { m_dwTimeOut = val; }
	/**
	* Reset time out with every io complete.if session time out.it will be kicked out.@see IsOnIdle function
	*/
	inline VOID				ResetTimeOut() { m_dwLastSyncTick = GetTickCount(); }

	/**
	* Access the timestamp.save the connection's build time.
	* FIXME:Remove it.we do not use it actully.
	*/
	DWORD					GetConnecttime() const { return m_connecttime; }
	/**
	* Access the timestamp.save the connection's build time.
	* FIXME:Remove it.we do not use it actully.
	*/
	void					SetConnecttime(DWORD val) { m_connecttime = val; }

	/**
	* Access IOCP handle.IO��ɶ˿ڵľ����
	*/
	void					SetIocpHandle(HANDLE val) { m_hIocp = val; }

	/**
	* Close this socket while session free.
	* ���ô˺����������ر�����
	* FIXME:Ӧ�ÿ�������shutdown�������
	*/
	VOID					CloseSocket();

private:
	BOOL				m_bRemove;
	SOCKET				m_socket;
	RecvBuffer*			m_pRecvBuffer;
	SendBuffer*			m_pSendBuffer;

	NetworkCallBack*	m_NetworkCallBack;

	SOCKADDR_IN			m_sockaddr;
	DWORD				m_dwIndex;

	unsigned short		m_usPort;
	DWORD				m_dwTimeOut;
	DWORD				m_dwLastSyncTick;
	DWORD				m_connecttime;

	
	OVERLAPPEDEX		m_sendIoData;///<these 3 private members are the "truly private".as a fact,i do not know how to encapsulate previous members.
	OVERLAPPEDEX		m_recvIoData;
	OVERLAPPEDEX		m_disconnectIoData;
	
	HANDLE				m_hIocp;
};

#endif
