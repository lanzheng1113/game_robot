#ifndef _DVDTSERVER_H
#define _DVDTSERVER_H
/**
* @file IOCPServer.h 定义IocpServer class.
* @author Vincent
* 
* @brief 服务器类。功能包括运行,初始化服务器。
* 
* about iohander iohander is center of the iocp tcp server.as you see.all the listen/bind/io thread are created in iohandler class.
* and session create.session manager.session destory also operatored in it.
* the iohaner is configure by struct IOHANDLER_DESC.it contains the tcp port,io thread max count etc.
*/
class IoHander;

struct tagIOHANDLER_DESC;
typedef tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;
/**
* @brief IocpServer服务器类
*
* 是IOCP网络服务器的最顶级抽象。
* 封装了ioHandle启动，关闭，初始化的过程。
* 封装初始化服务器，启动服务器，关闭服务器的过程。
*/
class IocpServer
{
public:
	IocpServer();
	~IocpServer();
public:
	/**
	* Run(),运行服务器。
	* @return 成功返回TRUE，否则返回FALSE.
	* @remark 仅仅是空壳子占用一个坑。总是返回TRUE。真正的工作在Init函数中已经完成。<B>FIXME:应该把Init函数的实现放到Run中。</B>
	*/
	BOOL		Run();
	/**
	* 关闭服务器，无返回值。
	*/
	void		ShutDown();
	/**
	* 根据pDesc初始化服务器。
	* @param pDesc IO配置描述结构体，服务器的主要配置（工作线程数量，缓冲区长度，最大吞吐量等等）。
	* @remark Init函数开启服务器IOCP线程，预定服务器所需内存资源。配置工作环境。执行完Init函数后，服务器就可以接受客户端的请求了。
	* @remark <B>FIXME:将listen过程应移动到Run函数中</B>
	*/
	BOOL		Init(LPIOHANDLER_DESC pDesc);
private:
	IoHander* m_ioHander;
	IOHANDLER_DESC* m_ioParameter;
};



#endif