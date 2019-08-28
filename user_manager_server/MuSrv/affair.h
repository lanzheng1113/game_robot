#pragma once
/**
* @file affair.h
* @brief ���������ඨ��
*
* �����������������IO������Ҫ����ҵ�����̡�
*/
class NetworkCallBack;

/**
* @brief �����������֯ҵ�����̡�
*/
class affair
{
public:
	affair(void);
	virtual ~affair(void);
public:
	/**
	* ���麯���ӿڣ����Ự���ر�ʱ����
	*/
	virtual VOID	OnSessionClosed() = 0;
	/**
	* �˽ӿ��յ�����ʱ�����á�
	* @param pMsg ���ݰ�ָ��
	* @param wSize ���ݰ���С
	* @remark ���ݵ����ݰ��Ѿ�ȥ������������ͷ
	*/
	virtual	VOID	OnDataRecv( BYTE *pMsg, WORD wSize ) = 0;

public:
	NetworkCallBack* GetNetworkCallback() const { return m_NetworkCallback; }
	void SetNetworkCallback(NetworkCallBack* val) { m_NetworkCallback = val; }

	/**
	* �������ݰ��ӿ�
	* @param pMsg ���ݰ�
	* @param wSize ���ݰ���С
	* @remark �������Զ�Ϊ���������ͷ���ͻ�����Ҫע�����ʱ2���ֽڵ����ݰ�ͷ��
	*/
	BOOL			Send( BYTE *pMsg, WORD wSize );
	/**
	* ���������ݰ��ӿ�Send
	*/
	BOOL			SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	/**
	* �رյ�ǰ�Ự�ӿ�
	*/
	BOOL			CloseSession();
	/**
	* ���ڱ����ظ�����CloseSession��Ϣ��
	* @remark �˽ӿ��Ѿ����� FIXME:<B>ɾ��֮�Լ��ٽӿ�</B>
	*/
	BOOL			GetCloseRequestPosted() const { return m_CloseRequestPosted; }
private:
	NetworkCallBack* m_NetworkCallback;///<����NetworkCallBack����
	BOOL			m_CloseRequestPosted;
};
