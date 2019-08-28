#ifndef _TESTSERVER_H
#define _TESTSERVER_H
#include "NetworkCallBack.h"

class DoValidateAffair;

class NVDSNetworkObject : public NetworkCallBack
{
public:
	NVDSNetworkObject();
	~NVDSNetworkObject();
public:
	virtual VOID	OnDisconnect();
	virtual	VOID	OnRecv( BYTE *pMsg, WORD wSize );
public:
	void OnSessionSetup();
protected:
private:
	DWORD m_dwRecvCount;
	DoValidateAffair* m_pDovalidateAffair;
};

BOOL InitSrvGlobalParameter();
void RunTestServer();
void ShutDownServer();

#endif