#pragma once
#include "affair.h"
#include <map>
class TableUserInfo;
class NetworkCallBack;
class DBM;
class DoValidateAffairMgr;
class COleDateTime;

class DoValidateAffair :
	public affair
{
public:
	DoValidateAffair(void);
	~DoValidateAffair(void);
public:
	typedef enum{
		DoValidateStep_NotBegin=0,
		DoValidateStep_1,
		DoValidateStep_2,
		DoValidateStep_3,
		DoValidateStep_4,
		DoValidateStep_5,
		DoValidateStep_6,
		DoValidateStep_7,
		DoValidateStep_OK=200,
		DoValidateStep_Failed=400,
	}CurrentDoValidateStep;

public:
	virtual VOID	OnSessionClosed();
	virtual	VOID	OnDataRecv( BYTE *pMsg, WORD wSize );

	BOOL			ShouldRemoved(){return m_bRemove;}
	VOID			Remove(){InterlockedExchange((LONG*)&m_bRemove,TRUE);}

	DWORD			GetLastCommTime() const { return m_LastCommTime; }
	void			SetLastCommTime(DWORD val) { m_LastCommTime = val; }

	TableUserInfo*		GetUserinfo() const { return m_pUserinfo; }

	BOOL Regiest() const { return m_bRegiest; }
	void Regiest(BOOL val) { m_bRegiest = val; }
	void			SetClientIp(const char* ip);
	TCHAR*			GetClientIp(){return (TCHAR*)m_szSessionIp;}
	COleDateTime*	GetTimeDoValidateOk() const { return m_pTimeDoValidateOk; }
	DoValidateAffair::CurrentDoValidateStep GetValidateStep() const { return m_doValidateStep; }

	BOOL Checked() const { return m_Checked; }
	void Checked(BOOL val) { m_Checked = val; }
private:
	VOID	EasyVerify(BYTE* pPacket,WORD wSize);
	WORD	EasyVerifyCheckMacs(const BYTE* pPacket,WORD PacketSize,OUT BYTE* sendBuffer,DWORD dwBufferMaxSize);
	VOID	DoValidate(BYTE* pPacket,WORD wSize);
	std::map<std::string,unsigned int> genClientFileInfo( BYTE * pMsg, WORD wSize );
private:
	TableUserInfo* m_pUserinfo;
	BOOL m_bRegiest;
	BOOL m_bRemove;
	CurrentDoValidateStep m_doValidateStep;
	DWORD m_LastCommTime;
	unsigned char m_XorSendBuf[0x20];
	unsigned char m_XorRecvBuf[0x20];
	DBM* m_pDbm;
	DoValidateAffairMgr* m_pDvdtAffairMgr;
	TCHAR m_szSessionIp[40];
	TCHAR m_szUserTableName[60];
	COleDateTime* m_pTimeDoValidateOk;
	BOOL m_Checked;
	BYTE* m_extraSendBuffer;
	int m_extraSendBufferSize;
	int m_curSendPos;
	TCHAR m_szProxyName[32];
};
