#ifndef _SESSIONPOOL_H_
#define _SESSIONPOOL_H_
/**
* @file SessionPool.h
* @author Vincent
* @date 7/27/2012
*
* @brief SessionPool 会话池是预定义的会话资源列表
*
* 当一个会话生成时，从会话池中申请会话资源。当一个会话被注销时被会话池回收。<B>简单地说就是【以内存开销换CPU开销】</B>
*/
#include <windows.h>
#include <vector>

using namespace std;

class session;
class SessionList;

/**
* @brief  SessionPool 会话池是预定义的会话资源列表
*/
class SessionPool
{
public:
	/**
	* 构造函数，创建会话列表
	* @param dwSize 列表总大小
	* @param dwSendBufferSize 发送缓冲区长度
	* @param dwRecvBufferSize 接收缓冲区长度
	* @param dwMaxPacketSize 数据包的最大大小
	* @param dwTimeOutTick session的超时时间
	* @param dwIndexStart 第一个session的id,创建session时id递增。
	* @remark FIXME:实际上并未用到session的id.
	*/
	SessionPool( DWORD dwSize, DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOutTick, DWORD dwIndexStart );
	/**
	* 析构函数
	*/
	~SessionPool();
	/**
	* 从SessionPool中请求一个session.
	* 因为是预先分配好内存资源的，所以十分迅速。只是简单从当前的session返回一个。
	* @return 返回可以用的session指针，如果没有可用的session则返回NULL.
	*/
	session*		Alloc();
	/**
	* 回收一个session到session Pool.
	* @param pSession 指向session的指针
	* @remark <B>FIXME:没有检测pSession的有效性</B>
	*/
	void			Free( session* pSession );
	/**
	* 获得session list的长度。
	* @return 返回当前可用session列表的长度
	*/
	int				GetLength();

private:
	VOID			Create( DWORD dwSize, DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwTimeOutTick, DWORD dwMaxPacketSize, DWORD dwIndexStart );

	DWORD			m_dwMaxSize;
	SessionList		*m_pList;
};

#endif
