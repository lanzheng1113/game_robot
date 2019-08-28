#include "StdAfx.h"
#include "MyUserMgr.h"
#include "util/File.h"
#include "util/Path.h"
#include "util/stringex.h"
#include "util/datetime.h"
#include "util/datetimespan.h"
#include "debugmessage.h"
#include <sstream>
#include <fstream>
#include <set>
#include "../common/AutoLock.h"
CLock_CS g_cs; 

#include "../common/AutoLock.h"

#ifndef _SRVIC
#include "../common/NetBase.h"
#include "../common/NVDSNetHeader.h"
extern SOCKET g_sComm;
#else
#include "../MuSrv/DBAccessor.h"
#include "../MuSrv/ADODB.H"
#include "../MuSrv/TLogger.h"
//在服务器端，用数据库实现接口。
#endif

#ifndef DATE_TIME_FORMATE
#define DATE_TIME_FORMATE "%d-%d-%d %d:%d:%d"
#endif

BOOL g_hasDirtyTask = FALSE;

MyUserMgr::MyUserMgr(void)
{
#ifdef _SRVIC
	m_dbm = NULL;
	hEventExit = CreateEvent(NULL,FALSE,FALSE,NULL);
#endif
	m_locker_cs = new CLock_CS();
}

MyUserMgr::~MyUserMgr(void)
{
}

BOOL MyUserMgr::load()
{
	std::string pa = Path::getApplicationDirPath();
#ifdef _SRVIC
	m_dbm = new DBAccessor();
	BOOL bCon = m_dbm->connectDB();
	if (!bCon){
		AfxMessageBox("连接数据库失败");
		ExitProcess(0);
	}

	std::list<CMyNineXUsr*> zhanghaoxinxis = m_dbm->loadAllUserToMemory();
	if (!zhanghaoxinxis.empty()){
		addNineXUsrs(zhanghaoxinxis);
	}else
		AfxMessageBox(_T("没有帐号信息哦。"));

	for (int i=0; i!=_usrs.size(); i++){
		DebugMessage( "第%d个,id = %s,type=%s,name=%s,harddiskid=%s,tm=%s",
			i+1,
			_usrs[i]->getAuthId().c_str(),
			_usrs[i]->getCardDaysType().c_str(),_usrs[i]->getName().c_str(),
			_usrs[i]->getHarddisk().c_str(),
			_usrs[i]->getDate_of_end().c_str());
	}

	return TRUE;
#else
	std::string pa1 = pa + "CEnables\\";
	File::createPath(pa1);

	std::list<std::string> wenjians = getFilesInDir(pa1);

	int ics = 1,itts = wenjians.size();
	for (std::list<std::string>::const_iterator it = wenjians.begin(); it != wenjians.end(); ++it){
		std::stringstream ss; ss << "读取文件" << *it << "：第" << ics++ << "个，共" << itts << "个。" ;
		if (String(*it).endsWith(".txt")){
			std::list<CMyNineXUsr*> zhanghaoxinxis = congwenjianduqu(*it);
			if (!zhanghaoxinxis.empty()){
				addNineXUsrs(zhanghaoxinxis);
			}
		}
	}

	for (int i=0; i!=_usrs.size(); i++){
		DebugMessage( "第%d个,id = %s,type=%s,name=%s,harddiskid=%s,tm=%s",
			i+1,_usrs[i]->getAuthId().c_str(),
			_usrs[i]->getCardDaysType().c_str(),_usrs[i]->getName().c_str(),
			_usrs[i]->getHarddisk().c_str(),
			_usrs[i]->getDate_of_end().c_str());
	}

	return TRUE;
#endif

}

std::list<std::string> MyUserMgr::getFilesInDir( const std::string& mulu )
{
	return File(mulu).getFileListRecursive();
}

std::list<CMyNineXUsr*> MyUserMgr::congwenjianduqu( const std::string& zhanghaowenjianming )
{
	std::ifstream ifs;
	std::list<CMyNineXUsr*> zhanghaoliebiao;
	ifs.open(zhanghaowenjianming.c_str());
	
	File f(zhanghaowenjianming);
	std::string sfname = f.getFileName();
	StringList sl = String(sfname).split(".");
	if (!sl.empty()){
		sfname = sl[0];
	}else{
		DebugMessage("获取用户名字失败了。");
		return zhanghaoliebiao;
	}
	
	if (ifs.is_open())
	{
		std::string line;
		int lines = 0;
		std::vector<std::string> str_lines;
		std::vector<std::string> tagBlock;
		while (getline(ifs,line)){
			DebugMessage("%d:%s",lines+1,line.c_str());
			String lineEx(line);	//剔除LINE中的空格
			line = lineEx.trim();
			str_lines.push_back(line);
			lines ++;
		}
		ifs.close();

		if (lines == 0){
			return zhanghaoliebiao;
		}else{
			int sz = str_lines.size();
			for (int i=0; i!=sz; i++){
				CMyNineXUsr* zhxx= lineToCMyNineXUsrObj(str_lines[i],sfname); //将行转换成授权信息对象。
				if (zhxx){
					zhanghaoliebiao.push_back(zhxx);
				}
			}
		}
	}else{
		char xzxxe[255] = {0};
		sprintf(xzxxe,"打开文件%s失败了LastError=%d。",zhanghaowenjianming.c_str(),GetLastError());
		MessageBox(NULL,xzxxe,NULL,MB_OK);
	}
	return zhanghaoliebiao;
}

CMyNineXUsr* MyUserMgr::lineToCMyNineXUsrObj( const std::string& line,const std::string& name )
{
	String infors(line);
	StringList li = infors.split("|");
	if (li.size() < 4){
		return NULL;
	}

	CMyNineXUsr* zhxx = new CMyNineXUsr;
	if (!zhxx){
		return NULL;
	}

	//格式，xo1xo2xo3xo|4xo5xo6xo7o9xo10xo11|2013-11-11_0:0:0 网卡序列号，硬盘序列号，到期时间。
	//062279883|31|未绑定|未绑定|未绑定
	//062279887|3|未绑定|未绑定|2013-10-11_0:0:0
	//123456789|11|xp1xp2xp3xp|4xp5xp6xp7xp9xpsxo1xpk|2013-11-11_0:0:0
	std::string kardId = li[0];
	std::string kardType = li[1];
	std::string harddisk = li[2];
	std::string dateend = li[3];
	
	zhxx->setName(name);
	zhxx->setAuthId(kardId);
	zhxx->setCardDaysType(kardType);
	zhxx->setHarddisk(harddisk);
	zhxx->setDate_of_end(dateend);
	
	return zhxx;
}

void MyUserMgr::addNineXUsrs( const std::list<CMyNineXUsr*>& NineXUsrs )
{
	for (std::list<CMyNineXUsr*>::const_iterator it=NineXUsrs.begin(); it != NineXUsrs.end(); ++it){
		_usrs.push_back(*it);
	}
}


std::vector<CMyNineXUsr*>  MyUserMgr::addNewUsr(int count,int days,const std::string& name)
{
#ifdef _SRVIC
	std::vector<CMyNineXUsr*> tempss;
	srand(GetTickCount());
	for (int i=0; i!=count; i++)
	{
		//创建账号
		//第一步，创建随即的卡号，9位数字
		char szKardId[10] = {0};
		do
		{
			int z = 0;
			while (z<9){
				szKardId[z++] = (char)(48 + rand()%10); /*ASCII 0-9*/
			}
			szKardId[9] = 0;
		}while (GetUsrInfo(szKardId) != NULL);

		//第二步，填写类型
		char szType[10] = {0};
		sprintf(szType,"%d",days);
		//填写空的与
		std::string harddiskId = unbinded_tag;
		//填写空的与
		std::string endTime = unbinded_tag;

		//申请一个新的对象，填充各个与
		CMyNineXUsr* bp = new CMyNineXUsr;
		bp->setName(name);
		bp->setAuthId(szKardId);
		bp->setCardDaysType(szType);
		bp->setDate_of_end(endTime);//写入数据库的时候需要判断一下是否是unbinded_tag，因为数据库里面的是时间格式，对应的，从数据库里读出的时候也需要判断一下。
		bp->setHarddisk(harddiskId);
		bp->setAccessSuccessCount(0);
		bp->setAccUseCount(0);
		bp->setIndex(-1); //写入数据库的时候需要判断这个值是否为-1，是-1表示为新值。
		bp->setIsDirty(true);
		bp->setReActiveTime(INIT_DB_TIMESTAMP);
		bp->setUserCreateTime(DateTime().toString());
		bp->setUserRegistTime(INIT_DB_TIMESTAMP);

		//保存对象到临时列表中。
		tempss.push_back(bp);

		//插入到总列表中
		{
			CAutoLock lc(g_cs);
			_usrs.push_back(bp);
		}
	}
	
	if (count != 0 && tempss.size()!=0){
		//saveUsrListByName(name);
		m_dbm->DBI_addNewUsr(tempss);
	}
	
	return tempss;
#else
	std::vector<CMyNineXUsr*> vec;
	PacketKaiKa kaiKa;

	PACKETGENERALHEAD hd;
	hd.CheckSum = 0;
	hd.GameType = 1;
	hd.GameVersion = 1;
	hd.HeadFlag = AffairType_Dovalidate;
	hd.HeadUserID = 0;
	hd.MesgFullType = MsgKaiKa;
	hd.PacketLen = sizeof(PacketKaiKa);
	hd.WgVersion = 1;

	kaiKa.pgh = hd;
	kaiKa.count = count;
	kaiKa.days = days;
	strncpy(kaiKa.name,name.c_str(),sizeof(kaiKa.name)); kaiKa.name[31] = 0;

	char sdb[2048] = {0};
	*(WORD*)sdb = sizeof(kaiKa);
	memcpy(sdb+2,&kaiKa,sizeof(kaiKa));

	int iResult = send(g_sComm,sdb,sizeof(kaiKa)+2,0);
	if (iResult != sizeof(kaiKa)+2)
	{
		MessageBox(NULL,"发送开卡数据包失败。开卡失败。",NULL,MB_OK);
		return vec;
	}else{
		char rcvb[4096] = {0};

		char recbufTemp[4096] = {0};
		iResult = 0;
		while (1)
		{
			int nowRecved = recv(g_sComm,recbufTemp,sizeof(recbufTemp),0);
			if (nowRecved == 0){
				MessageBox(NULL,"接收开卡结果失败了。",NULL,MB_OK);
				return vec;
			}else if (nowRecved == -1){
				MessageBox(NULL,"接收开卡结果失败了。",NULL,MB_OK);
				return vec;
			}else{
				DebugMessage("客户端:Recved %d bytes.",nowRecved);
				memcpy(rcvb+iResult,recbufTemp,nowRecved);
				iResult += nowRecved;
				if (iResult == *(WORD*)rcvb+2){
					break;
				}
			}
		}

// 		iResult = recv(g_sComm,rcvb,sizeof(rcvb),0);
// 		if (iResult == 0 || iResult == -1)
// 		{
// 			MessageBox(NULL,"接收开卡结果失败了。",NULL,MB_OK);
// 			return vec;
// 		}else
		{
			int bufc = iResult - 2;
			char* buf = rcvb + 2;
			ACKPACKETHEAD* pAck = (ACKPACKETHEAD*)buf;
			if (pAck->AckMesgFullType == AckMsgAdminKaikaFailed){
				MessageBox(NULL,"服务器报告开卡失败了。请稍后重试。",NULL,MB_OK);
				return vec;
			}
			buf += sizeof(ACKPACKETHEAD);

			int usrsc = *(WORD*)buf;buf += 2;
			int daysReturn = *(WORD*)buf;buf += 2;
			while (usrsc --){
				CMyNineXUsr* n = new CMyNineXUsr;
				char AuthId[10] = {0};
				memcpy(AuthId,buf,10);buf+=10;
				n->setAuthId(AuthId);
				vec.push_back(n);
			}
			return vec;
		}
	}
#endif
}

BOOL MyUserMgr::saveUsrListByName( const std::string& name )
{
	CAutoLock lc(g_cs);
	//首先从用户列表中生成一份只有名字为name参数的列表
	std::vector<CMyNineXUsr*> v;
	for (std::vector<CMyNineXUsr*>::const_iterator it=_usrs.begin(); it != _usrs.end(); ++it){
		if ((*it)->getName() == name){
			v.push_back(*it);
		}
	}

	if (v.empty())
	{
		return FALSE;
	}
	
	std::vector<std::string> vs;
	
	for (int i=0; i!=v.size(); i++)
	{
		std::string line = v[i]->getAuthId(); line += "|";
		line += v[i]->getCardDaysType(); line += "|";
		line += v[i]->getHarddisk(); line += "|";
		line += v[i]->getDate_of_end();

		vs.push_back(line);
	}

	//首先确认“name”.txt是否存在,如果不存在则创建它。
	std::string pa = Path::getApplicationDirPath();
	std::string pa1 = pa + "Enables\\";
	pa1 += name;
	pa1 += ".txt";

	File::createPath(pa1);
	//OK，现在文件一定存在。打开它。
	std::ofstream ofs;
	ofs.open(pa1.c_str());
	if (!ofs.is_open()){
		return FALSE; //打开失败了。
	}

	for (int i=0; i!=vs.size(); i++)
	{
		//逐行写入
		ofs << vs[i] << "\n";
	}
	ofs.close();

	return TRUE;
}

CMyNineXUsr* MyUserMgr::GetUsrInfo( const std::string& KardId )
{
	for (int i=0; i!=_usrs.size(); i++){
		if (_usrs[i]->getAuthId() == KardId){
			return _usrs[i];
		}
	}
	return NULL;
}

int MyUserMgr::unbindNoCondition( const std::string& kardId ) //返回1成功操作
{
#ifdef _SRVIC
	CAutoLock lc(g_cs);
	bool isunbinded = false;
	std::string nameToSave = "";
	for (int i=0; i!=_usrs.size(); i++){
		if (_usrs[i]->getAuthId() == kardId){
			if (_usrs[i]->getHarddisk() != unbinded_tag){
				_usrs[i]->setHarddisk(unbinded_tag);
				isunbinded = true;
				nameToSave = _usrs[i]->getName();
			}
		}
	}

	if (isunbinded){
		//saveUsrListByName(nameToSave);
		//saveDb();
		m_dbm->DBI_unbindNoCondition(kardId);
		return 0;
	}

	return 1;
#else
	char sendbuf[1024] = {0};
	PacketUnbind pu;
	pu.pgh.CheckSum = 0;
	pu.pgh.GameType = 1;
	pu.pgh.GameVersion = 1;
	pu.pgh.HeadFlag = AffairType_Dovalidate;
	pu.pgh.HeadUserID = 1;
	pu.pgh.MesgFullType = MsgUnbind;
	pu.isSub3Days = 0;
	strncpy(pu.AuthId,kardId.c_str(),sizeof(pu.AuthId));
	pu.AuthId[9] = 0;
	*(WORD*)sendbuf = sizeof(pu);
	memcpy(sendbuf+2,&pu,sizeof(pu));
	int isend = send(g_sComm,sendbuf,2+sizeof(pu),0);
	if (isend != 2+sizeof(pu)){
		MessageBox(NULL,"发送数据包失败了。",NULL,MB_OK);
		return 1;
	}else{
		char recvbuf[1024] = {0};
		int irecv = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
		if (irecv == 0 || irecv == -1){
			MessageBox(NULL,"接收数据包失败了。",NULL,MB_OK);
			return 1;
		}
		if (irecv != sizeof(ACKPACKETHEAD)+2)
		{
			MessageBox(NULL,"接收数据包失败了，大小不对。",NULL,MB_OK);
			return 1;
		}
		ACKPACKETHEAD* pAck = (ACKPACKETHEAD*)(recvbuf+2);
		if (pAck->AckMesgFullType == AckMsgUnbindSuccess){
			return 0;
		}else{
			return 1;
		}
	}
#endif
}

int MyUserMgr::addfivedaysall(){
	std::set<std::string> nametosave_set;
	for (int i=0; i!=_usrs.size(); i++){
		if (_usrs[i]->getDate_of_end() != unbinded_tag){
			if (basic_utils_for_UsrMgr::getRemainHours(_usrs[i]->getDate_of_end()) >= 28*24){
				continue;
			}
			_usrs[i]->setDate_of_end(basic_utils_for_UsrMgr::add5days(_usrs[i]->getDate_of_end()));
			nametosave_set.insert(_usrs[i]->getName());
		}
	}

	for (std::set<std::string>::const_iterator it = nametosave_set.begin(); it!= nametosave_set.end(); ++ it){
#ifdef _SRVIC
		m_dbm->DBI_AddFiveDays();
#else
		saveUsrListByName(*it);
#endif
	}

	return 1;
}

int MyUserMgr::unbindSub1Days( const std::string& kardId ,const std::string& proxyName)
{
#ifdef _SRVIC
	CAutoLock lc(g_cs);
 	bool isunbinded = false;
 	std::string nameToSave= "";
	std::string dateAfterSub;
 	for (int i=0; i!=_usrs.size(); i++){
		if (_usrs[i]->getAuthId() == kardId ){
			if(_usrs[i]->getHarddisk()== unbinded_tag){
				TLOG("[管理员:%s]用户%s已经是解绑状态了。解绑失败！",proxyName.c_str(),kardId.c_str());
				return 2;
			}

			DateTime dtLastUnbind = basic_utils_for_UsrMgr::fromString(_usrs[i]->getDate_of_last_unbind());
			DateTimeSpan span = DateTime() - dtLastUnbind;
			if (span.GetTotalHours() < 48)
			{
				TLOG("[管理员:%s]用户%s已经在48小时内解绑过一次了。解绑失败！",proxyName.c_str(),kardId.c_str());
				return 4;
			}

			if (basic_utils_for_UsrMgr::getRemainHours(_usrs[i]->getDate_of_end()) > 24*1){
				_usrs[i]->setHarddisk(unbinded_tag);
				dateAfterSub = basic_utils_for_UsrMgr::sub1days(_usrs[i]->getDate_of_end());
				TLOG("[管理员:%s]解绑用户ID=%s,解绑前的使用期限为%s，解绑后的使用期限为%s",proxyName.c_str(),kardId.c_str(),_usrs[i]->getDate_of_end().c_str(),dateAfterSub.c_str());
				_usrs[i]->setDate_of_end(dateAfterSub);
				_usrs[i]->setDate_of_last_unbind(DateTime().toString());
				nameToSave = _usrs[i]->getName();
				isunbinded = true;
			}else{
				TLOG("[管理员:%s]用户%s剩下的时间不足扣除1天。解绑失败！",proxyName.c_str(),kardId.c_str());
				return 3;
			}
 		}
	}

	if (isunbinded)
	{
		//saveUsrListByName(nameToSave);
		//saveDb();
		TLOG("[管理员:%s]成功，将解绑信息写入到数据库中,将%s的使用期限改为%s",proxyName.c_str(),kardId.c_str(),dateAfterSub.c_str());
		m_dbm->DBI_unbindSub1Day(kardId,dateAfterSub);
		return 0;
	}
	return 1;
#else
	char sendbuf[1024] = {0};
	PacketUnbind pu;
	pu.pgh.CheckSum = 0;
	pu.pgh.GameType = 1;
	pu.pgh.GameVersion = 1;
	pu.pgh.HeadFlag = AffairType_Dovalidate;
	pu.pgh.HeadUserID = 1;
	pu.pgh.MesgFullType = MsgUnbind;
	pu.isSub3Days = 1;
	strncpy(pu.AuthId,kardId.c_str(),sizeof(pu.AuthId));
	pu.AuthId[9] = 0;
	*(WORD*)sendbuf = sizeof(pu);
	memcpy(sendbuf+2,&pu,sizeof(pu));
	int isend = send(g_sComm,sendbuf,2+sizeof(pu),0);
	if (isend != 2+sizeof(pu)){
		MessageBox(NULL,"发送数据包失败了。",NULL,MB_OK);
		return 1;
	}else{
		char recvbuf[1024] = {0};
		int irecv = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
		if (irecv == 0 || irecv == -1){
			MessageBox(NULL,"接收数据包失败了。",NULL,MB_OK);
			return 1;
		}
		if (irecv != sizeof(ACKPACKETHEAD)+2)
		{
			MessageBox(NULL,"接收数据包失败了，大小不对。",NULL,MB_OK);
			return 1;
		}
		ACKPACKETHEAD* pAck = (ACKPACKETHEAD*)(recvbuf+2);
		if (pAck->AckMesgFullType == AckMsgUnbindSuccess){
			return 0;
		}else{
			return 1;
		}
	}
#endif
}

void MyUserMgr::deleteOutDateKards()
{
	CAutoLock lc(g_cs);
	std::set<std::string> names_erased;
	for (std::vector<CMyNineXUsr*>::const_iterator it = _usrs.begin(); it!=_usrs.end();)
	{
		CMyNineXUsr* xuser = *it;
		int year=0,month=0,day=0,hour=0,minute=0,second=0;
		if (xuser->getDate_of_end() == unbinded_tag){
			++it;
			continue;
		}
		sscanf(xuser->getDate_of_end().c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
		DateTime dEnd(day,month,year,hour,minute,second);
		DateTime dNow;
		DateTimeSpan dSpanEMN = dEnd - dNow;
		DateTimeSpan dSpanNME = dNow - dEnd;
		int emn = dSpanEMN.GetTotalSeconds();
		int nme = dSpanNME.GetTotalSeconds();
		if (emn > 0)
		{
			++it;
			continue;
		}else{
			//No,过期。
			names_erased.insert(xuser->getName());
			it = _usrs.erase(it);
		}
	}
	
// 	for (std::set<std::string>::const_iterator itset = names_erased.begin(); itset != names_erased.end(); ++itset){
// 		//saveUsrListByName(*itset);
// 	}
	saveDb();

}

BOOL MyUserMgr::saveDb()
{
	g_hasDirtyTask = TRUE;
	return TRUE;
}

void MyUserMgr::RegistUser( const std::string& kardId,const std::string& hardid )
{
#ifdef _SRVIC
	m_dbm->DBI_registKard(kardId,hardid);
#endif
}

void MyUserMgr::bindUser( const std::string& kardId,const std::string& hardid )
{
#ifdef _SRVIC
	m_dbm->DBI_bindUser(kardId,hardid);
#endif
}

std::vector<CMyNineXUsr*> MyUserMgr::getCopy()
{
	CAutoLock aulocker(*m_locker_cs);
	std::vector<CMyNineXUsr*> tcopy = _usrs;
	return tcopy;
}

void MyUserMgr::deleteAcc( const CString& m_strToDelAcc )
{
#ifdef _SRVIC
	m_dbm->DBI_DelAcc((LPCTSTR)m_strToDelAcc);
#endif
}

BOOL MyUserMgr::CheckProxyUser( const std::string& proxyAcc,const std::string& proxyPswd )
{
#ifdef _SRVIC
	return m_dbm->DBI_CheckProxyUser(proxyAcc,proxyPswd);
#else
	return FALSE;
#endif
}

int basic_utils_for_UsrMgr::getRemainHours( const std::string& dateTimeEnd )
{
	int year=0,month=0,day=0,hour=0,minute=0,second=0;
	sscanf(dateTimeEnd.c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
	DateTime d(day,month,year,hour,minute,second);
	DateTime now;
	DateTimeSpan span = d - now;
	return span.GetTotalHours();
}

std::string basic_utils_for_UsrMgr::sub3days( const std::string& dateTimeEnd )
{
	int year=0,month=0,day=0,hour=0,minute=0,second=0;
	sscanf(dateTimeEnd.c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
	DateTime d(day,month,year,hour,minute,second);
	DateTimeSpan Min3(3,0,0,0);
	DateTime dSub3Days = d - Min3;
	CString s;
	s.Format(DATE_TIME_FORMATE,
		dSub3Days.getYear(),dSub3Days.getMonth(),dSub3Days.getDay(),
		dSub3Days.getHour(),dSub3Days.getMinute(),dSub3Days.getSecond());
	return std::string((LPCTSTR)s);
}

std::string basic_utils_for_UsrMgr::sub1days( const std::string& dateTimeEnd )
{
	int year=0,month=0,day=0,hour=0,minute=0,second=0;
	sscanf(dateTimeEnd.c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
	DateTime d(day,month,year,hour,minute,second);
	DateTimeSpan Min1(1,0,0,0);
	DateTime dSub1Days = d - Min1;
	CString s;
	s.Format(DATE_TIME_FORMATE,
		dSub1Days.getYear(),dSub1Days.getMonth(),dSub1Days.getDay(),
		dSub1Days.getHour(),dSub1Days.getMinute(),dSub1Days.getSecond());
	return std::string((LPCTSTR)s);
}

std::string basic_utils_for_UsrMgr::add5days( const std::string& dateTimeEnd )
{
	int year=0,month=0,day=0,hour=0,minute=0,second=0;
	sscanf(dateTimeEnd.c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
	DateTime d(day,month,year,hour,minute,second);
	DateTimeSpan add5(5,0,0,0);
	DateTime dSub1Days = d + add5;
	CString s;
	s.Format(DATE_TIME_FORMATE,
		dSub1Days.getYear(),dSub1Days.getMonth(),dSub1Days.getDay(),
		dSub1Days.getHour(),dSub1Days.getMinute(),dSub1Days.getSecond());
	return std::string((LPCTSTR)s);
}

DateTime basic_utils_for_UsrMgr::fromString( const std::string& dateTimeConvert )
{
	int year=0,month=0,day=0,hour=0,minute=0,second=0;
	sscanf(dateTimeConvert.c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
	DateTime d(day,month,year,hour,minute,second);
	return d;
}
