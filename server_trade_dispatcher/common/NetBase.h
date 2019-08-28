#ifndef _NETBASE_H_
#define _NETBASE_H_

//=============================================================================================================================
/// ��Ŷ ���
//=============================================================================================================================
typedef struct tagPACKET_HEADER
{
	WORD	size;		/// ���� ��Ŷ�� ũ��
} PACKET_HEADER;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}
#endif

class NetworkCallBack;
typedef NetworkCallBack* (*fnCallBackCreateAcceptedObject)();
typedef VOID (*fnCallBackDestroyAcceptedObject)( NetworkCallBack *pNetworkObject );

typedef struct tagIOHANDLER_DESC
{
	USHORT								usListenPort;
	DWORD								dwMaxAcceptSession;				//
	DWORD								dwSendBufferSize;				//
	DWORD								dwRecvBufferSize;				//
	DWORD								dwTimeOut;						//
	DWORD								dwMaxPacketSize;				//
	DWORD								dwNumberOfIoThreads;			//
	fnCallBackCreateAcceptedObject		fnCreateAcceptedObject;			//
	fnCallBackDestroyAcceptedObject		fnDestroyAcceptedObject;		//
} IOHANDLER_DESC, *LPIOHANDLER_DESC;


#define heart 0x02fc
#define heart_ack 0x03fd
#endif