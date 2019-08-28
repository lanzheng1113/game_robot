#ifndef _DVDTSERVER_H
#define _DVDTSERVER_H
/**
* @file IOCPServer.h ����IocpServer class.
* @author Vincent
* 
* @brief �������ࡣ���ܰ�������,��ʼ����������
* 
* about iohander iohander is center of the iocp tcp server.as you see.all the listen/bind/io thread are created in iohandler class.
* and session create.session manager.session destory also operatored in it.
* the iohaner is configure by struct IOHANDLER_DESC.it contains the tcp port,io thread max count etc.
*/
class IoHander;

struct tagIOHANDLER_DESC;
typedef tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;
/**
* @brief IocpServer��������
*
* ��IOCP������������������
* ��װ��ioHandle�������رգ���ʼ���Ĺ��̡�
* ��װ��ʼ�����������������������رշ������Ĺ��̡�
*/
class IocpServer
{
public:
	IocpServer();
	~IocpServer();
public:
	/**
	* Run(),���з�������
	* @return �ɹ�����TRUE�����򷵻�FALSE.
	* @remark �����ǿտ���ռ��һ���ӡ����Ƿ���TRUE�������Ĺ�����Init�������Ѿ���ɡ�<B>FIXME:Ӧ�ð�Init������ʵ�ַŵ�Run�С�</B>
	*/
	BOOL		Run();
	/**
	* �رշ��������޷���ֵ��
	*/
	void		ShutDown();
	/**
	* ����pDesc��ʼ����������
	* @param pDesc IO���������ṹ�壬����������Ҫ���ã������߳����������������ȣ�����������ȵȣ���
	* @remark Init��������������IOCP�̣߳�Ԥ�������������ڴ���Դ�����ù���������ִ����Init�����󣬷������Ϳ��Խ��ܿͻ��˵������ˡ�
	* @remark <B>FIXME:��listen����Ӧ�ƶ���Run������</B>
	*/
	BOOL		Init(LPIOHANDLER_DESC pDesc);
private:
	IoHander* m_ioHander;
	IOHANDLER_DESC* m_ioParameter;
};



#endif