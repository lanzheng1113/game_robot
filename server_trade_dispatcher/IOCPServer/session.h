#ifndef _SESSION_H
#define _SESSION_H
/**
* @file session.h 会话封装了一个网络连接
* @author Vincent
* @date 2012-7-26
* @brief 网络类io连接类
*
* session类是IOCP服务器模块中对网络连接的封装，多个session被ioHandle调度执行收发动作。
* 作为网络IO的调度单元，执行IO具体操作的对象。
* 它通过NetworkCallBack向上传递Recv通知，并通过它下传递Send请求（通过填充它的send缓冲区）。
* 通过PreSend，PreRecv操作内置的RecvBuffer,SendBuffer执行收发动作，
* 可以通过session id辨识唯一的session，也可以通过session socket辨识。
* 对于iocp可以见iocp的介绍：<a href=http://blog.csdn.net/jifengszf/article/details/1896839>JeffShen的专栏</a>
*/
class SendBuffer;
class RecvBuffer;
class NetworkCallBack;
class session;

#define SEND_POSTED			1///<网络io请求类型：发送
#define RECV_POSTED			2///<网络io请求类型：接收
#define ACCEPT_POSTED		3///<网络io请求类型：接受连接
#define DISCONNECT_POSTED	4///<网络io请求类型：断开连接

/**
* 用于返回IOCP结果的异步操作结构体。必须作为OVERLAPPED的派生类。
* @see WSASend
* @see WSARecv
*/
struct OVERLAPPEDEX : public OVERLAPPED
{
	DWORD		dwIoSize;	///<io数量
	DWORD		dwFlags;	///<ioFlag
	DWORD		dwOperationType;	///<操作类型SEND_POSTED，RECV_POSTED，ACCEPT_POSTED，DISCONNECT_POSTED之一
	session*	pSession;	///<发出该io请求的session
};

/**
* define _cbk_fun 标示一个函数是一个回调函数，没有其他的意义。
*/
#ifndef _cbk_fun
#define _cbk_fun
#endif

/**
* @brief session会话封装了一个网络连接
*
* session类是IOCP服务器模块中对网络连接的封装，多个session被ioHandle调度执行收发动作。
* 作为网络IO的调度单元，执行IO具体操作的对象。
* 它通过NetworkCallBack向上传递Recv通知，并通过它下传递Send请求（通过填充它的send缓冲区）。
* 通过PreSend，PreRecv操作内置的RecvBuffer,SendBuffer执行收发动作，
* 可以通过session id辨识唯一的session，也可以通过session socket辨识。
* 对于io完成端口的具体介绍，可以见<a href=http://blog.csdn.net/jifengszf/article/details/1896839>JeffShen的专栏</a>
*/
class session
{
public:
	/**
	* 构造函数
	* @param dwSendBufferSize 发送缓冲区大小。
	* @param dwRecvBufferSize 接收缓冲区大小。
	* @param dwMaxPacketSize 数据包允许的最大大小。
	* @param dwTimeOut 超时时间。
	* FIXME：严谨起见应该将默认的构造函数设为私有。
	*/
	session( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut );
	/**
	* 默认析构函数
	*/
	~session();
protected:
public:
	/**
	* 初始化函数，使用session进行io操作之前（第一次或者回收利用时）必须调用一次初始化函数。
	*/
	VOID					Init();

	/**
	* Post a send request to iocp socket.
	* 不同于Send( BYTE *pMsg, WORD wSize )和SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )仅仅是写缓冲区,PreSend将缓冲区内的数据写入网络。
	* 可以理解为Send和SendEx是一个Write操作，而PreSend是一个refresh操作。
	* @return 成功返回TRUE，即使缓冲区没有数据也返回TRUE，发送数据发生网络错误返回FALSE.
	*/
	BOOL					PreSend();
	/**
	* Post a recv request to iocp socket
	* 向io完成端口投递一个RECV请求。当它被响应时将调用RecvBuffer的Completion完成写入数据。
	*/
	BOOL					PreRecv();
	/**
	* Set remove flag to remove this from active session list.
	* remove标记决定一个会话是否应该被从活动会话列表中删除。
	*/
	VOID					Remove() { InterlockedExchange( (long*)&m_bRemove, TRUE ); }
	/**
	* Write data to send buffer.(maybe it need a better function name.)
	* FIXME：需要一个更好的名字。实际上PreSend才是真正的执行send操作!
	*/
	BOOL					Send( BYTE *pMsg, WORD wSize );	
	/**
	* Write data to send buffer.(maybe it need a better function name.)
	* FIXME：需要一个更好的名字。实际上PreSend才是真正的执行send操作!
	*/
	BOOL					SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );

	VOID					BindNetworkCallBack(NetworkCallBack* cbk);///<bind network callback to transmit io result.
	VOID					UnbindNetworkCallBack();///<unbind network callback

	/**
	* 回调函数，用于传递OnReceive结果。执行后，如果缓冲区里有一个数据包则读取它并向NetworkCallBack传递传递OnReceive，如果没有则返回TRUE不执行任何操作，出错返回FALSE.
	* FIXME:可以将dwMaxPacketSize作为对象成员以简化函数，dwMaxPacketSize总是等于构造session时传入的dwRecvBufferSize。
	* @param dwMaxPacketSize 接收数据包允许的最大大小。
	* @return 执行成功返回TRUE，如果接收缓冲区没有数据也返回TRUE。执行过程中如果发送错误则返回FALSE.
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
	* Access IOCP handle.IO完成端口的句柄。
	*/
	void					SetIocpHandle(HANDLE val) { m_hIocp = val; }

	/**
	* Close this socket while session free.
	* 调用此函数以主动关闭连接
	* FIXME:应该考虑主动shutdown的情况。
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
