#pragma once
/**
* @file NetworkCallBack.h 定义NetworkCallBack
* @brief NetworkCallBack高层次io接口
*
* Class NetworkCallBack网络回调类是对网络IO的封装，对于每一个连接都有对应的一个NetworkCallBack实例。该实例用于处理send过程和接受recv通知。
* @author Vincent
* @date 2012/7/20
*/
class session;

/**
* @brief NetworkCallBack定义高层次io接口.
*
* NetworkCallBack提供了一个简洁的接口供业务逻辑使用。使用NetworkCallBack良好的封装，可以几乎完全无视底层实现。
* 使用NetworkCallBack的接口可以方便写业务相关的代码，而不用考虑真正的send/recv/diconnect过程。
* <B>注意如果需要对send/recv进行特定错误的特殊处理，需要自行添加代码。</B>
*/
class NetworkCallBack
{
public:
	/**
	* 默认构造函数
	*/
	NetworkCallBack(void);
	/**
	* NetworkCallBack类用于作为基类设计，因此它的析构函数是virtual的。
	*/
	virtual ~NetworkCallBack(void);
public:
	/**
	* 获取这个网络对象对应的底层session对象【connection对象】
	*/
	session *	GetSession() { return m_pSession; }
	/**
	* 设置网络对象对应的底层session对象
	*/
	VOID	SetSession( session *pSession ) { m_pSession = pSession; }
	/**
	* 获得网络peer的IP
	*/
	char*	GetIP();
public:
	/**
	* 虚函数，回调函数
	* 当底层session关闭时被调用。
	*/
	virtual VOID	OnDisconnect() = 0;
	/**
	* 虚函数，回调函数
	* 当session收到一个完整的数据包时被调用。
	* @param pMsg:数据包数据，不包括两个字节的数据头，该数据头在调用这个回调时已经被排除。
	* @param wSize:数据包的长度。
	*/
	virtual	VOID	OnRecv( BYTE *pMsg, WORD wSize ) = 0;
	/**
	* 虚函数，回调函数
	* 底层记录事件时调用。
	* @param pszLog:事件描述
	*/
	virtual VOID	OnLogString( char *pszLog ) {}
	/**
	* 向网络发送一个数据包
	* @param pMsg:数据包
	* @param wSize:数据包长度
	*/
	BOOL			Send( BYTE *pMsg, WORD wSize );
	/**
	* 向网络发送多个数据段，这些数据段组成了一个数据包
	* @param dwNumberOfMessages:数据包个数
	* @param ppMsg:数据数组
	* @param pwSize:数据长度数组，与数据数组对应。
	*/
	BOOL			SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	/**
	* 主动断开与网络的连接,如果正常断开将会收到OnDisconnect()消息。
	*/
	VOID			Disconnect();
private:
	session			*m_pSession;

};
