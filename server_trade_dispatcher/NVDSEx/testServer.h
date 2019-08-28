#ifndef _TESTSERVER_H
#define _TESTSERVER_H

#include <string>
using std::string;

#include "NetworkCallBack.h"

class DoValidateAffair;
class CTDatabaseObject;
class CTradeAffair;

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
	CTradeAffair* m_TradeAffair;
};



BOOL InitSrvGlobalParameter();
BOOL RunServer();
void ShutDownServer();
#endif