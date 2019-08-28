#pragma once
/**
* @file affair.h
* @brief 事务对象基类定义
*
* 事务对象，区别于网络IO对象主要负责业务流程。
*/
class NetworkCallBack;

/**
* @brief 事务对象负责组织业务流程。
*/
class affair
{
public:
	affair(void);
	virtual ~affair(void);
public:
	/**
	* 纯虚函数接口，当会话被关闭时调用
	*/
	virtual VOID	OnSessionClosed() = 0;
	/**
	* 此接口收到数据时被调用。
	* @param pMsg 数据包指针
	* @param wSize 数据包大小
	* @remark 传递的数据包已经去除网络层的数据头
	*/
	virtual	VOID	OnDataRecv( BYTE *pMsg, WORD wSize ) = 0;

public:
	NetworkCallBack* GetNetworkCallback() const { return m_NetworkCallback; }
	void SetNetworkCallback(NetworkCallBack* val) { m_NetworkCallback = val; }

	/**
	* 发送数据包接口
	* @param pMsg 数据包
	* @param wSize 数据包大小
	* @remark 网络层会自动为它添加数据头。客户端需要注意接收时2个字节的数据包头。
	*/
	BOOL			Send( BYTE *pMsg, WORD wSize );
	/**
	* 见发送数据包接口Send
	*/
	BOOL			SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	/**
	* 关闭当前会话接口
	*/
	BOOL			CloseSession();
	/**
	* 用于避免重复发送CloseSession消息。
	* @remark 此接口已经作废 FIXME:<B>删除之以减少接口</B>
	*/
	BOOL			GetCloseRequestPosted() const { return m_CloseRequestPosted; }
private:
	NetworkCallBack* m_NetworkCallback;///<内置NetworkCallBack对象
	BOOL			m_CloseRequestPosted;
};
