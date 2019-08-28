#ifndef _IOHANDER_H
#define _IOHANDER_H
/**
* @file IoHander.h 定义IoHander类
* @author:Paul Vincent.
* @brief class IoHandler是服务器的核心模块。网络的接收线程，发送线程，各工作线程均在这里创建。并且根据配置创建预分配的资源"session poll"。IO端口创建，监听,bind等核心操作均在这里完成。
*/
#include "sessionlist.h"

class SessionPool;
class session;
class IocpServer;
class NetworkCallBack;

struct tagIOHANDLER_DESC;
typedef tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;

#define EXTRA_ACCEPTEX_NUM 10

typedef NetworkCallBack* (*fnCallBackCreateAcceptedObject)();
typedef VOID (*fnCallBackDestroyAcceptedObject)( NetworkCallBack *pNetworkObject );

/**
* @brief IoHander类封装iocp的send/recv/accept/disconnect等io例程
*
* IoHandler是服务器的核心模块。网络的接收线程，发送线程，各工作线程均在这里创建。
* 并且根据配置创建预分配的资源"session poll"。
* IO端口创建，监听,bind等核心操作均在这里完成。
*/
class IoHander
{
public:
	IoHander();
	~IoHander();
public:
	/**
	* Init函数，根据初始化IO handler。
	* @param pIOCPServer: 遗留代码，实际无作用。
	* @param lpDesc:存储服务器运行参数的结构体。
	*/
	BOOL		Init( IocpServer *pIOCPServer, LPIOHANDLER_DESC lpDesc );
	VOID		Clear();
protected:
	/**
	* send_thread：用于（并发）发送数据包的线程【从会话队列中发起io send请求】
	* @param thdParam class IoHander实例this.
	*/
	static UINT send_thread(LPVOID thdParam);
	/**
	* accept_thread 用于接受连接的线程
	* @param thdParam class IoHander实例this.
	*/
	static UINT accept_thread(LPVOID thdParam);
	/**
	* io_thread 用于处理iocp的io请求线程,分配io请求，是iocp服务器的核心线程。
	* @param thdParam class IoHander实例this.
	*/
	static UINT io_thread(LPVOID thdParam);
	/**
	* UpdateThread 处理session list中死亡和即将死亡的线程，并处理活动session的Recv缓冲区（如符合数据大小约定则向上发送Recv通知）。
	* @param thdParam IoHander实例this.
	*/
	static UINT UpdateThread(LPVOID thdParam);
private:
	void		ProcessActiveSessions();	//process the sessions in list.kick off sessions which is time out.
	void		kickdDeadSessions();		//kick out session which Remove flag is set.
	session*	AllocAcceptSession();		//alloc session.to make it better,consider to pre-alloc a session list.and alloc from session list
	VOID		FreeSession( session *pSession );//free session.to improve it.consider to pre-alloc a session list,and free to session list.
private:
	SessionPool			*m_pAcceptSessionPool;
	SOCKET m_listenSocket;
	HANDLE m_hIOCP;
	BOOL m_bExit;
	HANDLE m_hThdAccept;
	HANDLE m_hThdUpdate;
	HANDLE m_hThdSend;
	SessionList			m_AcceptedSessionList;
	IOHANDLER_DESC*	m_serverParameter;
	DWORD m_dwMaxPacketSize;
	fnCallBackCreateAcceptedObject		m_fnCreateAcceptedObject;
	fnCallBackDestroyAcceptedObject		m_fnDestroyAcceptedObject;
};
#endif