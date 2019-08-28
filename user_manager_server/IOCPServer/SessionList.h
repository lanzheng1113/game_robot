#ifndef _SESSIONLIST_H_
#define _SESSIONLIST_H_
/**
* @file sessionList.h 会话列表类
* @brief 封装了会话列表
*
* std::list<session*>类型的列表简单封装。
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
* @brief SessionList类是public std::list<session*>的继承类。注意它是session*类型的列表。
*
* 修改SessionList时注意不要在SessionList中添加虚函数，因为std::list<session*>的虚构函数不是虚拟的。
* 如果使用std::list<session*> * 类型的指针指向SessionList时，当销毁对象时会发生资源泄露。
* 具体说明可以搜索<B>Effective C++ 条款14: 确定基类有虚析构函数</B>
*/
class SessionList : public std::list<session*>
{
public:
	/**
	* 默认构造函数
	*/
	SessionList();
	/**
	* 默认析构函数
	*/
	~SessionList();

	VOID Clear();
	/**
	* 资源保护锁，锁定
	*/
	inline VOID		Lock()		{ EnterCriticalSection( &m_cs ); }
	/**
	* 资源保护锁，解除锁定
	*/
	inline VOID		Unlock()	{ LeaveCriticalSection( &m_cs ); }

private:
	CRITICAL_SECTION m_cs;
};

#endif
