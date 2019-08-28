#ifndef _IOHANDER_H
#define _IOHANDER_H
/**
* @file IoHander.h ����IoHander��
* @author:Paul Vincent.
* @brief class IoHandler�Ƿ������ĺ���ģ�顣����Ľ����̣߳������̣߳��������߳̾������ﴴ�������Ҹ������ô���Ԥ�������Դ"session poll"��IO�˿ڴ���������,bind�Ⱥ��Ĳ�������������ɡ�
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
* @brief IoHander���װiocp��send/recv/accept/disconnect��io����
*
* IoHandler�Ƿ������ĺ���ģ�顣����Ľ����̣߳������̣߳��������߳̾������ﴴ����
* ���Ҹ������ô���Ԥ�������Դ"session poll"��
* IO�˿ڴ���������,bind�Ⱥ��Ĳ�������������ɡ�
*/
class IoHander
{
public:
	IoHander();
	~IoHander();
public:
	/**
	* Init���������ݳ�ʼ��IO handler��
	* @param pIOCPServer: �������룬ʵ�������á�
	* @param lpDesc:�洢���������в����Ľṹ�塣
	*/
	BOOL		Init( IocpServer *pIOCPServer, LPIOHANDLER_DESC lpDesc );
	VOID		Clear();
protected:
	/**
	* send_thread�����ڣ��������������ݰ����̡߳��ӻỰ�����з���io send����
	* @param thdParam class IoHanderʵ��this.
	*/
	static UINT send_thread(LPVOID thdParam);
	/**
	* accept_thread ���ڽ������ӵ��߳�
	* @param thdParam class IoHanderʵ��this.
	*/
	static UINT accept_thread(LPVOID thdParam);
	/**
	* io_thread ���ڴ���iocp��io�����߳�,����io������iocp�������ĺ����̡߳�
	* @param thdParam class IoHanderʵ��this.
	*/
	static UINT io_thread(LPVOID thdParam);
	/**
	* UpdateThread ����session list�������ͼ����������̣߳�������session��Recv����������������ݴ�СԼ�������Ϸ���Recv֪ͨ����
	* @param thdParam IoHanderʵ��this.
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