#ifndef _SESSIONLIST_H_
#define _SESSIONLIST_H_
/**
* @file sessionList.h �Ự�б���
* @brief ��װ�˻Ự�б�
*
* std::list<session*>���͵��б�򵥷�װ��
* @author Vincent
* @date 2012/7/27
*/
#include <windows.h>
#include <process.h>
#include <list>

class session;

typedef std::list<session*>			SESSION_LIST;
typedef SESSION_LIST::iterator		SESSION_LIST_ITER;

/**
* @brief SessionList����public std::list<session*>�ļ̳��ࡣע������session*���͵��б�
*
* �޸�SessionListʱע�ⲻҪ��SessionList������麯������Ϊstd::list<session*>���鹹������������ġ�
* ���ʹ��std::list<session*> * ���͵�ָ��ָ��SessionListʱ�������ٶ���ʱ�ᷢ����Դй¶��
* ����˵����������<B>Effective C++ ����14: ȷ������������������</B>
*/
class SessionList : public std::list<session*>
{
public:
	/**
	* Ĭ�Ϲ��캯��
	*/
	SessionList();
	/**
	* Ĭ����������
	*/
	~SessionList();

	VOID Clear();
	/**
	* ��Դ������������
	*/
	inline VOID		Lock()		{ EnterCriticalSection( &m_cs ); }
	/**
	* ��Դ���������������
	*/
	inline VOID		Unlock()	{ LeaveCriticalSection( &m_cs ); }

private:
	CRITICAL_SECTION m_cs;
};

#endif
