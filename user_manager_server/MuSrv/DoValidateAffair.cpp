#include "StdAfx.h"
#include "DoValidateAffair.h"
#include "DvdtAffairMgr.h"
#include "../common/DebugMessage.h"
#include "NetworkCallBack.h"
#include "DvdtEnDecrypt.h"
#include "../common/DvdtMessageDef.h"
#include "../common/DBdef.h"
#include "../common/NetBase.h"
#include "DvdtAffairMgr.h"
#include "../common/constdefine.h"
#include "../common/EventIdDef.h"
#include "../common/ErrorMesgDef.h"
#include "atlcomtime.h"
#include "../common/nvdsnetheader.h"
#include "../common/LoginerErrMsgDef.h"
#include "../common/crypt.h"
#include <libUtil/util/Logger.h>
#include "../common/MyUserMgr.h"
#include "util/datetime.h"
#include "util/datetimespan.h"
#include "OffsetMgr.h"
#include "FileUpDownLoad.h"
#include "../common/MyUtil.h"
#include "../common/MyNineXUsr.h"
#include "UserTransfer.h"
#include "../common/Notice.h"
#include "TLogger.h"

//#include "strsafe.h"

FileUpDownLoad fileud;

extern MyUserMgr g_mum;
DoValidateAffair::DoValidateAffair(void)
:affair()
,m_pUserinfo(NULL)
,m_pTimeDoValidateOk(NULL)
{
	m_Checked = FALSE;
	m_extraSendBuffer = NULL;
	m_extraSendBufferSize = 0;
	m_pDbm = NULL;
	m_pUserinfo = NULL;
	m_bRemove = FALSE;
	m_bRegiest = FALSE;
	m_doValidateStep = DoValidateStep_NotBegin;
	m_LastCommTime = GetTickCount();//affair created when data received.so,last set CommTime = getTicketCount;
	memset(m_XorSendBuf,0,sizeof(m_XorSendBuf));
	memset(m_XorRecvBuf,0,sizeof(m_XorRecvBuf));
	m_pDvdtAffairMgr = getDvdtAffairMgrPtr();
	memset(m_szSessionIp,0,sizeof(m_szSessionIp));
	memset(m_szUserTableName,0,sizeof(m_szUserTableName));
	m_pTimeDoValidateOk = new COleDateTime();
}

DoValidateAffair::~DoValidateAffair(void)
{
	SAFE_DELETE(m_pTimeDoValidateOk);
	SAFE_DELETE(m_extraSendBuffer);
}

VOID DoValidateAffair::OnDataRecv( BYTE *pMsg, WORD wSize )
{
	if (!m_bRegiest)
	{
// 		if(!(m_pUserinfo && m_pTimeDoValidateOk)){//alloc memory failed when object construct!
// 			CloseSession();//it is not a valide object.
// 			return;
// 		}
		m_pDvdtAffairMgr->RegisterAffair(this);//register the affair to affair manager.can not fail.
		m_bRegiest = TRUE;//Do not care about multi-thread.the RECV-Request is posted one by one in our iocp-server.
	}

	BYTE sendbf[2048] = {0};
	PACKETGENERALHEAD* p = (PACKETGENERALHEAD*)pMsg;
	switch(p->MesgFullType){
		case MsgGetSeed:
			{
				if (wSize != sizeof(PacketGetSeed))
				{
					CloseSession();
					return;
				}

				if (p->GameVersion != 2)
				{
					//在这里检测版本信息
				}
				if (p->WgVersion != 2)
				{
					//在这里检测版本信息
				}

				PacketGetSeed* pgs = (PacketGetSeed*)pMsg;
				//卡号
				char KardId[64] = {0};
				memset(KardId,0,sizeof(KardId));
				SimpleXor_Crype((char*)KardId,(char*)pgs->id,sizeof(pgs->id));

				//服务器流程：
				//step 1 SimpleXor_Crype解密出卡号
				//setp 2 验证是否有卡号存在。
				//      如果不存在，返回1个WORD = 1
				//		否则传回SEED
				CMyNineXUsr* xuser = g_mum.GetUsrInfo(KardId);
				if (!xuser){
					*(WORD*)sendbf = 1;
					Send(sendbf,2);
					LOG_ERROR("#9.不存在的ID‘%s’，源自IP:%s",KardId,m_szSessionIp);
					return;
				}

				if (xuser->getLow_seed() == 0 && xuser->getHigh_seed() == 0)
				{
					srand(GetTickCount());
					xuser->setLow_seed(rand());
					xuser->setHigh_seed(rand());
					return;
				}

				ACK_GetSeed ack_gs;
				ack_gs.ack.AckMesgFullType = AckMsgGetSeed;
				ack_gs.ack.DataSizeRemain = 0;
				ack_gs.ack.GameType = 0;
				ack_gs.ack.GameVersion = 2;
				ack_gs.ack.LastPacket = 1;
				ack_gs.ack.WgVersion = 0;
				ack_gs.seedHigh = xuser->getHigh_seed();
				ack_gs.seedLow = xuser->getLow_seed();

				Send((BYTE*)&ack_gs,sizeof(ack_gs));

				return;
			}
		case 0:
			{
				if (wSize != sizeof(PacketS1)){
					CloseSession();
					return;
				}

				if (p->GameVersion != 2)
				{
					//在这里检测版本信息
				}
				if (p->WgVersion != 2)
				{
					//在这里检测版本信息
				}

				PacketS1* b = (PacketS1*)pMsg;
				
				//卡号
				char KardId[64] = {0};
				memset(KardId,0,sizeof(KardId));
				SimpleXor_Crype((char*)KardId,(char*)b->id,sizeof(b->id));
				
				//1st.检测是否卡号存在
				CMyNineXUsr* xuser = g_mum.GetUsrInfo(KardId);
				if (!xuser){
					*(WORD*)sendbf = 1;
					Send(sendbf,2);
					LOG_ERROR("#9.不存在的ID‘%s’，源自IP:%s",KardId,m_szSessionIp);
					return;
				}

				BYTE btInfoEn[128] = {0};
				memcpy(btInfoEn,b->sqm_netcard_diskid,128);

				//DWORD dwSeed = (DWORD)atoi(KardId);
				DWORD dwSeed = xuser->getHigh_seed();
				unsigned char encryptb1 = (unsigned char)(dwSeed & 0x000000ff);
				unsigned char encryptb2 = (unsigned char)((dwSeed & 0x0000ff00) >> 8);
				unsigned char encryptb3 = (unsigned char)((dwSeed & 0x00ff0000) >> 16);
				unsigned char encryptb4 = (unsigned char)((dwSeed & 0xff000000) >> 24);

				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb1;
				}

				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb2;
				}

				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb3;
				}

				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb4;
				}

				BYTE info[128] = {0};
				memset(info,0,sizeof(info));
				SimpleXor_Crype((char*)info,(char*)btInfoEn,128);
				
				CString aaa;
				CString bbb;
				for (int i=0; i!=125; i++){
					 aaa.Format("%02X",info[i]); 
					 bbb+= aaa;
				}

				std::string bx = (LPCTSTR)bbb;
				if (bx == "" || bx == unbinded_tag)
				{
					CloseSession();
					return;
				}

				//2nd.检测是否卡过期。
				String sDe = xuser->getDate_of_end();
				sDe.remove("\r");
				sDe.remove("\n");
				sDe.remove(" ");
				if (sDe == unbinded_tag){
					//这个卡是尚未绑定的卡。
					int iDays = atoi(xuser->getCardDaysType().c_str());
					if (iDays !=0 && iDays !=-1){
						DateTime d;
						DateTimeSpan dspan(iDays,0,0,0);
						d = d + dspan;
						//DateTimeSpan( int lDays,int nHours,int nMins,int nSecs );
						std::stringstream ss;
						//2013-11-11_0:0:0
						ss << d.getYear() << "-" << d.getMonth() << "-" << d.getDay() 
							<<" " << d.getHour() << ":" << d.getMinute() << ":" << d.getSecond();

						xuser->setDate_of_end(ss.str());
						xuser->setHarddisk(bx);
						//g_mum.saveUsrListByName(xuser->getName()); //保存到文件。
						//g_mum.saveDb();
						g_mum.RegistUser(xuser->getAuthId(),xuser->getHarddisk());
						LOG_INFO("#绑定.用户%s绑定了卡%s,HDID=%s,IP=%s",xuser->getName().c_str(),KardId,bx.c_str(),m_szSessionIp);
					}
				}else{
					int year=0,month=0,day=0,hour=0,minute=0,second=0;
					sscanf(xuser->getDate_of_end().c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
					DateTime dEnd(day,month,year,hour,minute,second);
					DateTime dNow;
					DateTimeSpan dSpanEMN = dEnd - dNow;
					DateTimeSpan dSpanNME = dNow - dEnd;
					int emn = dSpanEMN.GetTotalSeconds();
					int nme = dSpanNME.GetTotalSeconds();
					if (emn > 0)
					{
						//Ok有效。
						if (xuser->getHarddisk() == unbinded_tag){
							xuser->setHarddisk(bx);
							//g_mum.saveUsrListByName(xuser->getName()); //保存到文件。
							//g_mum.saveDb();
							g_mum.bindUser(xuser->getAuthId(),xuser->getHarddisk());
							LOG_INFO("#重新绑定.用户%s绑定了卡%s,HDID=%s,IP=%s",xuser->getName().c_str(),KardId,bx.c_str(),m_szSessionIp);
						}
					}else{
						//No,过期。
						*(WORD*)sendbf = 2;
						Send(sendbf,2);
						LOG_ERROR("#8.用户%s使用了一次过期的卡%s,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
						return;
					}
				}
				
 				//3rd.检测是否硬件信息正确。
				PacketS1RetOffset ccav;
				ccav.ErrorCode = 0;
				ccav.len = sizeof(ccav);
				
 				if (bx == xuser->getHarddisk()){
					LOG_INFO("用户%s的卡%s通过了验证，发送正确的偏移,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
					OffsetMgr::getInstance().genTrueOffset(ccav.Offset);
				}else{
					std::string bx1;
					std::string hd1;
					std::string srv_data_banding = xuser->getHarddisk();
					//情况1，客户端上传的带网卡，但是服务器的数据没有网卡数据。
					if (bx.length() > 12){
						bx1 = bx.substr(12,bx.length()-12); //上传的砍掉头12个字节。
						if (bx1.length() > 128 && srv_data_banding.length()>128)
						{
							std::string clt = bx1.substr(0,128);
							std::string srv = srv_data_banding.substr(0,128);
							if (srv == clt){
								LOG_INFO("用户%s的卡%s通过了验证，但是客户端此时成功获得了网卡ID。而绑定时未获得网卡ID,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
								xuser->setHarddisk(bx);
								//g_mum.saveUsrListByName(xuser->getName()); //保存到文件。
								//g_mum.saveDb();
								g_mum.bindUser(xuser->getAuthId(),xuser->getHarddisk());
								LOG_INFO("#纠错重绑(+网卡).用户%s绑定了卡%s,IP=%s",xuser->getName().c_str(),bx.c_str(),m_szSessionIp);

								OffsetMgr::getInstance().genTrueOffset(ccav.Offset);
								Send((BYTE *)&ccav,sizeof(ccav));
								break;
							}
						}
					}

					//情况2，客户端上传的不带网卡，但是服务器的数据带有12字节网卡。
					if (srv_data_banding.length() > 12){
						std::string srv1 = srv_data_banding.substr(12,srv_data_banding.length()-12); //服务器砍掉12字节。
						if (bx.length() > 128 && srv1.length() > 128){
							std::string srv = srv1.substr(0,128);
							std::string clt = bx.substr(0,128);
							if (srv == clt)
							{
								LOG_INFO("用户%s的卡%s通过了验证，但是客户端未上传网卡ID。,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
								OffsetMgr::getInstance().genTrueOffset(ccav.Offset);
								Send((BYTE *)&ccav,sizeof(ccav));
								break;
							}
						}	
					}
					//情况3，服务器没有绑定网卡。需要砍掉客户端的头24个字节
					if (bx.length() > 24)
					{
						bx1 = bx.substr(24,bx.length()-24);
						if (bx1.length() > 128 && srv_data_banding.substr().length()>128)
						{
							std::string clt = bx1.substr(0,128);
							std::string srv = srv_data_banding.substr(0,128);
							if (srv == clt){
								LOG_INFO("用户%s的卡%s通过了验证，但是客户端此时成功获得了网卡ID。而绑定时未获得网卡ID,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
								xuser->setHarddisk(bx);
								//g_mum.saveUsrListByName(xuser->getName()); //保存到文件。
								//g_mum.saveDb();
								g_mum.bindUser(xuser->getAuthId(),xuser->getHarddisk());
								LOG_INFO("#纠错重绑(+24网卡).用户%s绑定了卡%s,IP=%s",xuser->getName().c_str(),bx.c_str(),m_szSessionIp);
								OffsetMgr::getInstance().genTrueOffset(ccav.Offset);
								Send((BYTE *)&ccav,sizeof(ccav));
								break;
							}
						}
					}

					//情况4，客户端上传的不带网卡，但是服务器的数据带有24字节的网卡
					if (srv_data_banding.length() > 24)
					{
						std::string srv1 = srv_data_banding.substr(24,srv_data_banding.length()-24);
						if (bx.length() > 128 && srv1.length()>128)
						{
							std::string srv = srv1.substr(0,128);
							std::string clt = bx.substr(0,128);
							if (srv == clt)
							{
								LOG_INFO("用户%s的卡%s通过了验证，但是客户端未上传网卡ID(24字节)。,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
								OffsetMgr::getInstance().genTrueOffset(ccav.Offset);
								Send((BYTE *)&ccav,sizeof(ccav));
								break;
							}
						}
					}
					LOG_INFO("用户%s的卡%s没有通过验证\n    上传的机器ID=%s,\n    原来的机器ID=%s,IP=%s",
																	xuser->getName().c_str(),
																	KardId,
																	bx.c_str(),
																	xuser->getHarddisk().c_str(),
																	m_szSessionIp);
					OffsetMgr::getInstance().genFalseOffset(ccav.Offset);
					ccav.ErrorCode = 1;
				}
				Send((BYTE *)&ccav,sizeof(ccav));
			}		
			break;
		case MsgCheckBaiTanUser:
			{
				if (wSize != sizeof(PacketChkBaiTan))
				{
					CloseSession();
					break;
				}

				PacketChkBaiTan* bt = (PacketChkBaiTan*)pMsg;
				char KardId[64] = {0};
				memset(KardId,0,sizeof(KardId));
				SimpleXor_Crype((char*)KardId,(char*)bt->id,sizeof(bt->id));

				//1st.检测是否卡号存在
				CMyNineXUsr* xuser = g_mum.GetUsrInfo(KardId);
				if (!xuser){
					*(WORD*)sendbf = 1;
					Send(sendbf,2);
					LOG_ERROR("#上摊位DLL验证失败##########.\r\n    不存在的ID%s，源自IP:%s",KardId,m_szSessionIp);
					return;
				}

				if (xuser->getDate_of_end() == unbinded_tag){
					//这个卡是尚未绑定的卡。
					*(WORD*)sendbf = 3;
					Send(sendbf,2);
					LOG_INFO("#上摊位DLL验证失败##########\r\n    用户%s使用了未绑定的卡%s,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
				}

				int year=0,month=0,day=0,hour=0,minute=0,second=0;
				sscanf(xuser->getDate_of_end().c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
				DateTime dEnd(day,month,year,hour,minute,second);
				DateTime dNow;
				DateTimeSpan dSpanEMN = dEnd - dNow;
				DateTimeSpan dSpanNME = dNow - dEnd;
				int emn = dSpanEMN.GetTotalSeconds();
				int nme = dSpanNME.GetTotalSeconds();

				if (emn > 0){
					//Ok有效。
				}else{
					//No,过期。
					*(WORD*)sendbf = 2;
					Send(sendbf,2);
					LOG_ERROR("#上摊位DLL验证失败##########\r\n    用户%s使用了一次过期的卡%s,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
					return;
				}

				PacketS1RetOffset ccav;
				ccav.ErrorCode = 0;
				ccav.len = sizeof(ccav);
				OffsetMgr::getInstance().genBaiTanOffset(ccav.Offset);
				Send((BYTE *)&ccav,sizeof(ccav));
				break;
			}
		case 1:
			{
				if (wSize != sizeof(PacketS1)){
					CloseSession();
					return;
				}

				if (p->GameVersion != 1)
				{
					//在这里检测版本信息
				}
				if (p->WgVersion != 1)
				{
					//在这里检测版本信息
				}

				PacketS1* b = (PacketS1*)pMsg;

				//卡号
				char KardId[64] = {0};
				memset(KardId,0,sizeof(KardId));
				SimpleXor_Crype((char*)KardId,(char*)b->id,sizeof(b->id));

				BYTE btInfoEn[128] = {0};
				memcpy(btInfoEn,b->sqm_netcard_diskid,128);

				DWORD dwSeed = (DWORD)atoi(KardId);
				unsigned char encryptb1 = (unsigned char)(dwSeed & 0x000000ff);
				unsigned char encryptb2 = (unsigned char)((dwSeed & 0x0000ff00) >> 8);
				unsigned char encryptb3 = (unsigned char)((dwSeed & 0x00ff0000) >> 16);
				unsigned char encryptb4 = (unsigned char)((dwSeed & 0xff000000) >> 24);


				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb1;
				}

				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb2;
				}

				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb3;
				}

				for (int i = 0; i!=128; i++)
				{
					btInfoEn[i] = btInfoEn[i] ^ encryptb4;
				}

				//机器码
				BYTE info[128] = {0};
				memset(info,0,sizeof(info));
				SimpleXor_Crype((char*)info,(char*)btInfoEn,128);

				CString aaa;
				CString bbb;
				for (int i=0; i!=128; i++){
					aaa.Format("%02X",info[i]); 
					bbb+= aaa;
				}

				std::string bx = (LPCTSTR)bbb;

				//1st.检测是否卡号存在
				CMyNineXUsr* xuser = g_mum.GetUsrInfo(KardId);
				if (!xuser){
					*(WORD*)sendbf = 1;
					Send(sendbf,2);
					LOG_ERROR("#随机验证失败##########.\r\n    不存在的ID%s，源自IP:%s",KardId,m_szSessionIp);
					return;
				}

				//2nd.检测是否卡过期。
				if (xuser->getDate_of_end() == unbinded_tag){
					//这个卡是尚未绑定的卡。
					LOG_INFO("#随机验证失败##########\r\n    用户%s使用了未绑定的卡%s,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
				}else{
					int year=0,month=0,day=0,hour=0,minute=0,second=0;
					sscanf(xuser->getDate_of_end().c_str(),DATE_TIME_FORMATE,&year,&month,&day,&hour,&minute,&second);
					DateTime dEnd(day,month,year,hour,minute,second);
					DateTime dNow;
					DateTimeSpan dSpanEMN = dEnd - dNow;
					DateTimeSpan dSpanNME = dNow - dEnd;
					int emn = dSpanEMN.GetTotalSeconds();
					int nme = dSpanNME.GetTotalSeconds();
					if (emn > 0){
						//Ok有效。
					}else{
						//No,过期。
						*(WORD*)sendbf = 2;
						Send(sendbf,2);
						LOG_ERROR("#随机验证失败##########\r\n    用户%s使用了一次过期的卡%s,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
						return;
					}
				}
				//3rd.检测是否硬件信息正确。
				std::string dx = xuser->getHarddisk();
				if (bx == xuser->getHarddisk()){
					LOG_INFO("#随机验证####用户%s的卡%s通过了验证,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp);
					*(WORD*)sendbf = 4;
					Send(sendbf,2);
				}else{
					if (dx.length() >12){
						std::string cx = dx.substr(12,dx.length()-12);
						//DebugMessage("cutx=%s\npstx=%s",cx.c_str(),bx.c_str());
						std::string strSrv;
						std::string strClt;
						if (cx.length()>=128 && bx.length()>=128){
							strClt = cx.substr(0,128);
							strSrv = bx.substr(0,128);
						}else{
							strClt = cx;
							strSrv = bx;
						}

						if (strSrv == strClt){
							LOG_INFO("#随机验证####用户%s的卡%s通过了验证,IP=%s",xuser->getName().c_str(),KardId,m_szSessionIp); //针对Win7下取网卡ID失败的情况。
							*(WORD*)sendbf = 4;
							Send(sendbf,2);
						}else{
							LOG_INFO("#随机验证##########用户%s的卡%s没有通过验证\r\n	\
									 服务器保留的机器码=%s \r\n\
									 上传的机器码=%s \r\n\
									 ,IP=%s \
									 ##########",xuser->getName().c_str(),KardId,xuser->getHarddisk().c_str(),bx.c_str(),m_szSessionIp);

							*(WORD*)sendbf = 3;
							Send(sendbf,2);
						}
					}else{
						LOG_INFO("#随机验证##########用户%s的卡%s没有通过验证\r\n	\
								 服务器保留的机器码=%s \r\n\
								 上传的机器码=%s \r\n\
								 ,IP=%s\
								 ##########",xuser->getName().c_str(),KardId,xuser->getHarddisk().c_str(),bx.c_str(),m_szSessionIp);

						*(WORD*)sendbf = 3;
						Send(sendbf,2);
					}
				}
			}
			break;
		case MsgAdminUnbindUsrLogin:
			{
				if (wSize != sizeof(PacketProxyPswdCheck))
				{
					break;
				}
				PacketProxyPswdCheck* papc = (PacketProxyPswdCheck*)pMsg;
				char acc[16] = {0};
				char pswd[16] = {0};
				memcpy(acc,papc->acc,sizeof(papc->acc));
				memcpy(pswd,papc->pswd,sizeof(papc->pswd));
				acc[15] = 0;
				pswd[15] = 0;
				String stracc(acc);
				String strpswd(pswd);
				stracc.remove(" ");
				strpswd.remove(" ");
				if (g_mum.CheckProxyUser(stracc,strpswd)){
					m_Checked = TRUE;
				}
				
				if (m_Checked)
				{
					TLOG("###解绑客户端%s登录成功###，源自IP:%s",acc,m_szSessionIp);
					strcpy(m_szProxyName,acc);
					ACKPACKETHEAD ackPapc;
					ackPapc.AckMesgFullType = AckMsgAdminCheckOk;
					ackPapc.DataSizeRemain = 0;
					ackPapc.GameType = 0;
					ackPapc.GameVersion = 1;
					ackPapc.LastPacket = 1;
					ackPapc.WgVersion = 1;
					Send((BYTE*)&ackPapc,sizeof(ackPapc));
				}else{
					TLOG("###解绑客户端%s登录失败###，源自IP:%s",stracc.c_str(),m_szSessionIp);
					CloseSession();
				}
				break;
			}
			break;
		case MsgAdminCheckPswd:
			{
				//这个消息验证远程开卡用户的合法性。
				if (wSize != sizeof(PacketAdminPswdCheck))
				{
					break;
				}
				PacketAdminPswdCheck* papc = (PacketAdminPswdCheck*)pMsg;
				char pswd[8] = {0};
				memcpy(pswd,papc->pswd,sizeof(papc->pswd));
				pswd[7] = 0;
				if (!strcmp(pswd,"5200000")){
					m_Checked = TRUE;
				}

				if (m_Checked)
				{
					ACKPACKETHEAD ackPapc;
					ackPapc.AckMesgFullType = AckMsgAdminCheckOk;
					ackPapc.DataSizeRemain = 0;
					ackPapc.GameType = 0;
					ackPapc.GameVersion = 1;
					ackPapc.LastPacket = 1;
					ackPapc.WgVersion = 1;
					Send((BYTE*)&ackPapc,sizeof(ackPapc));
				}else{
					CloseSession();
				}
				break;
			}
			break;
		case MsgNeedUserInfo:
			{
				//这个消息是远程开卡用户请求数据。
				if (!m_Checked){
					CloseSession();
					break;
				}

				std::vector<CMyNineXUsr*> usersCpy = g_mum.getCopy();
				BOOL bTranslateVec = CUserTransfer::getInstance().FromNineXUserVec(usersCpy);
				m_extraSendBufferSize = CUserTransfer::getInstance().GetCountOfUserInfo() * sizeof(UserInfo);

				if (m_extraSendBufferSize == 0 || !bTranslateVec){
					ACKPACKETHEAD ackPapc;
					ackPapc.AckMesgFullType = AckMsgAdminAllUserFileSame;
					ackPapc.DataSizeRemain = 0;
					ackPapc.GameType = 0;
					ackPapc.GameVersion = 1;
					ackPapc.LastPacket = 1;
					ackPapc.WgVersion = 1;
					Send((BYTE*)&ackPapc,sizeof(ackPapc));
					break;
				}

				m_curSendPos = 0;
				if (m_extraSendBuffer)
				{
					delete [] m_extraSendBuffer;
					m_extraSendBuffer = NULL;
				}

				m_extraSendBuffer = new BYTE[m_extraSendBufferSize];
				if (!m_extraSendBuffer)
				{
					CloseSession();
					break;
				}
				
				
				memcpy(m_extraSendBuffer,CUserTransfer::getInstance().GetUserInfo(),m_extraSendBufferSize);
				DebugMessage("服务器：Total bytes %d",m_extraSendBufferSize);
				BYTE sendb[4000] = {0};
				if (m_extraSendBufferSize > 3000)
				{
					//send more than once
					ACKPACKETHEAD head;
					head.AckMesgFullType = AckMsgAdminPostUserData;
					head.DataSizeRemain = m_extraSendBufferSize-3000;
					DebugMessage("服务器：Post 3000 remain %d",m_extraSendBufferSize-3000);
					head.GameType = 0;
					head.GameVersion = 1;
					head.LastPacket = 0;
					head.WgVersion = 1;
					m_curSendPos = 3000;

					memcpy(sendb,&head,sizeof(head));
					memcpy(sendb+sizeof(head),m_extraSendBuffer,3000);
					Send(sendb,3000+sizeof(head));
					break;
				}else{
					//send once.
					ACKPACKETHEAD head;
					head.AckMesgFullType = AckMsgAdminPostUserData;
					head.DataSizeRemain = 0;
					head.GameType = 0;
					head.GameVersion = 1;
					head.LastPacket = 1;
					head.WgVersion = 1;

					memcpy(sendb,&head,sizeof(head));
					memcpy(sendb+sizeof(head),m_extraSendBuffer,m_extraSendBufferSize);
					Send(sendb,m_extraSendBufferSize+sizeof(head));
					m_curSendPos = m_extraSendBufferSize;
					break;
				}
			}
			break;
		case MsgKaiKa:
			{
				if (!m_Checked){
					CloseSession();
				}
				if (wSize != sizeof(PacketKaiKa))
				{
					DebugMessage("错误的大小。开卡信息。");
					CloseSession();
					break;
				}

				PacketKaiKa* pKaika = (PacketKaiKa*)pMsg;
				std::vector<CMyNineXUsr*>vret = g_mum.addNewUsr(pKaika->count,pKaika->days,pKaika->name);
				if (vret.empty()){
					DebugMessage("开卡失败！");
					ACKPACKETHEAD ack;
					ack.AckMesgFullType = AckMsgAdminKaikaFailed;
					ack.DataSizeRemain = 0;
					ack.GameType = 1;
					ack.GameVersion = 1;
					ack.LastPacket = 1;
					ack.WgVersion =1;
					Send((BYTE*)&ack,sizeof(ack));
				}else{
					LogKaiKaResult(vret);
					ACKPACKETHEAD ack;
					ack.AckMesgFullType = AckMsgAdminKaikaSuccessed;
					ack.DataSizeRemain = 0;
					ack.GameType = 1;
					ack.GameVersion = 1;
					ack.LastPacket = 1;
					ack.WgVersion =1;
					char sendbuf[3048] = {0};
					memcpy(sendbuf,&ack,sizeof(ack));
					*(WORD*)(sendbuf + sizeof(ack)) = (WORD)vret.size();
					*(WORD*)(sendbuf + sizeof(ack) + 2) = (WORD)pKaika->days;
					for (int i = 0; i!= vret.size(); i++){
						char aid[10] = {0};
						strncpy(aid,vret[i]->getAuthId().c_str(),sizeof(aid)); aid[9] = 0;
						memcpy(sendbuf + sizeof(ack) + 4 + i*10,aid,10);
					}
					Send((BYTE*)sendbuf,sizeof(ack) + 4 + vret.size()*10);
				}
				break;
			}
		case MsgUnbind:
			{
 				if (!m_Checked){
 					DebugMessage("not checked user!");
 					CloseSession();
 				}
 				if (wSize != sizeof(PacketUnbind))
 				{
 					DebugMessage("not right packet size!");
 					CloseSession();
 					break;
 				}
 
 				PacketUnbind* pu = (PacketUnbind*)pMsg;
 				int iresult = 0;
  				if (pu->isSub3Days)
				{
					DebugMessage("减使用时间解绑");
					iresult = g_mum.unbindSub1Days(pu->AuthId,"admin");
  				}else{
					DebugMessage("无条件解绑");
  					iresult = g_mum.unbindNoCondition(pu->AuthId);
  				}
				TLOG("###【控制端解绑】userid=%s###，源自IP:%s",pu->AuthId,m_szSessionIp);
 				//LOG_ERROR("###【控制端解绑】userid=%s###，源自IP:%s",pu->AuthId,m_szSessionIp);
 				DebugMessage("unbind userid = %s",pu->AuthId);
 				ACKPACKETHEAD ackP;
 				if (iresult == 0)
 				{
 					ackP.AckMesgFullType = AckMsgUnbindSuccess;
 					ackP.DataSizeRemain  = 0;
 					ackP.GameType = 1;
 					ackP.GameVersion = 1;
 					ackP.LastPacket = 1;
 					ackP.WgVersion = 1;
 					DebugMessage("unbind successed");
 				}else{
 					ackP.AckMesgFullType = AckMsgUnbindFailed;
 					ackP.DataSizeRemain  = 0;
 					ackP.GameType = 1;
 					ackP.GameVersion = 1;
 					ackP.LastPacket = 1;
 					ackP.WgVersion = iresult;
 					DebugMessage("unbind not success!");
 				}
 				Send((BYTE*)&ackP,sizeof(ackP));
 				break;
			}
		case MsgProxyUnbind:
			{
				if (!m_Checked){
					DebugMessage("not checked user!");
					CloseSession();
				}
				if (wSize != sizeof(PacketUnbind))
				{
					DebugMessage("not right packet size!");
					CloseSession();
					break;
				}

				PacketUnbind* pu = (PacketUnbind*)pMsg;
				int iresult = 0;
				DebugMessage("减使用时间解绑");
				iresult = g_mum.unbindSub1Days(pu->AuthId,m_szProxyName);
				DebugMessage("unbind userid = %s",pu->AuthId);
				ACKPACKETHEAD ackP;
				if (iresult == 0)
				{
					TLOG("###【代理解绑】%s成功解绑了账号%s###，源自IP:%s",m_szProxyName,pu->AuthId,m_szSessionIp);
					ackP.AckMesgFullType = AckMsgUnbindSuccess;
					ackP.DataSizeRemain  = 0;
					ackP.GameType = 1;
					ackP.GameVersion = 1;
					ackP.LastPacket = 1;
					ackP.WgVersion = 1;
					DebugMessage("unbind successed");
				}else{
					TLOG("###【代理解绑】%s试图解绑账号%s，但是失败了。原因ID=%d###，源自IP:%s",m_szProxyName,pu->AuthId,iresult,m_szSessionIp);
					ackP.AckMesgFullType = AckMsgUnbindFailed;
					ackP.DataSizeRemain  = 0;
					ackP.GameType = 1;
					ackP.GameVersion = 1;
					ackP.LastPacket = 1;
					ackP.WgVersion = iresult;
					DebugMessage("unbind not success!");
				}
				Send((BYTE*)&ackP,sizeof(ackP));
				break;
			}
		case MsgMoreUserInfo:
			{
				if (!m_Checked){
					CloseSession();
				}
				//这个消息是远程用户请求更多未传输完成的数据。这个消息带一个已传输总大小的参数。这样就可以从下一个位置开始选了。
				DWORD dwDataSizeRemain = m_extraSendBufferSize - m_curSendPos;
				DebugMessage("服务器[MsgMoreUserInfo]:dwDataSizeRemain = %d",dwDataSizeRemain);
				if (dwDataSizeRemain <=0){
					ACKPACKETHEAD head;
					head.AckMesgFullType = AckMsgAdminNoMoreUserData;
					head.DataSizeRemain = 0;
					head.GameType = 0;
					head.GameVersion = 1;
					head.LastPacket = 1;
					head.WgVersion = 1;
					Send((BYTE*)&head,sizeof(head));
					DebugMessage("send user data.no more user data.");
					break;
				}

				BYTE sendb[4000] = {0};
				if (dwDataSizeRemain>3000)
				{
					ACKPACKETHEAD head;
					head.AckMesgFullType = AckMsgAdminPostUserData;
					head.DataSizeRemain = m_extraSendBufferSize-3000;
					head.GameType = 0;
					head.GameVersion = 1;
					head.LastPacket = 0;
					head.WgVersion = 1;
					memcpy(sendb,&head,sizeof(head));
					memcpy(sendb+sizeof(head),m_extraSendBuffer+m_curSendPos,3000);
					Send(sendb,3000+sizeof(head));
					DebugMessage("send user data.%d bytes.",3000+sizeof(head));
					m_curSendPos = m_curSendPos+3000;
					break;
				}else{
					ACKPACKETHEAD head;
					head.AckMesgFullType = AckMsgAdminPostUserData;
					head.DataSizeRemain = 0;
					head.GameType = 0;
					head.GameVersion = 1;
					head.LastPacket = 1;
					head.WgVersion = 1;

					memcpy(sendb,&head,sizeof(head));
					memcpy(sendb+sizeof(head),m_extraSendBuffer+m_curSendPos,dwDataSizeRemain);
					Send(sendb,dwDataSizeRemain+sizeof(head));
					m_curSendPos = m_extraSendBufferSize;
					DebugMessage("send last bat user data.%d bytes.",dwDataSizeRemain+sizeof(head));
					break;
				}
			}
			break;
		case MsgRemoteUpdateOffset:
			{
				DebugMessageA("zzz0");
				if (wSize != sizeof(PacketRemoteUpdateOffset)){
					DebugMessageA("zzz1");
					CloseSession();
				}
				PacketRemoteUpdateOffset* p = (PacketRemoteUpdateOffset*)pMsg;
				OffsetMgr::getInstance().remoteUpdate(p->Offset,_countof(p->Offset));
				ACKPACKETHEAD head;
				head.AckMesgFullType = AckMsgRemoteUpdateOk;
				head.DataSizeRemain = 0;
				head.GameType = 0;
				head.GameVersion = 1;
				head.LastPacket = 1;
				head.WgVersion = 1;
				Send((BYTE*)&head,sizeof(head));
			}
			break;
		case MsgGetNotice:
			{
				DebugMessageA("get remote notice.");
				if (wSize != sizeof(PacketGetNotice)){
					DebugMessageA("bad size notice");
					CloseSession();
				}
				PacketGetNotice* p = (PacketGetNotice*)pMsg;
				if (CNotice::getInstance().getCrc() == p->noticeCrc)
				{
					ACKPACKETHEAD ack;
					ack.AckMesgFullType = AckMsgNoticeNoNewer;
					ack.DataSizeRemain = 0;
					ack.GameType = 0;
					ack.GameVersion = 0;
					ack.LastPacket = 1;
					ack.WgVersion = 0;
					Send((BYTE*)&ack,sizeof(ack));
				}else{
					ACKPACKET_SENDNOTICE ack_sendNotice;
					ack_sendNotice.ack.AckMesgFullType = AckMsgHasNewNotice;
					ack_sendNotice.ack.DataSizeRemain = 0;
					ack_sendNotice.ack.GameType = 0;
					ack_sendNotice.ack.GameVersion = 0;
					ack_sendNotice.ack.LastPacket = 1;
					ack_sendNotice.ack.WgVersion = 0;

					std::string notice = CNotice::getInstance().getContent();;
					int lenOfNotice = notice.length();
					if (lenOfNotice > 2500){
						lenOfNotice = 2500; //最大长度为2500
					}

					ack_sendNotice.NoticeCharLen = lenOfNotice + 1; //添加一个NULL符号。
					DWORD TotalLen = sizeof(ack_sendNotice)+ack_sendNotice.NoticeCharLen;

					BYTE* byteToSendBuffer = new BYTE[TotalLen];
					if (NULL == byteToSendBuffer)
					{
						CloseSession();
						break;
					}
					memset(byteToSendBuffer,0,TotalLen);
					memcpy(byteToSendBuffer,&ack_sendNotice,sizeof(ack_sendNotice)); //头
					strncpy((char*)(byteToSendBuffer+sizeof(ack_sendNotice)),notice.c_str(),lenOfNotice+1); //lenOfNotice是notice的长度，需要加一用于装NULL符。

					Send((BYTE*)byteToSendBuffer,ack_sendNotice.NoticeCharLen+sizeof(ack_sendNotice));
					delete [] byteToSendBuffer;
				}
				break;
			}
		default:
			break;
	}
}


VOID DoValidateAffair::OnSessionClosed()
{
	if (!m_bRegiest)
	{
		//Fatal error,if the affair receive this notify,it means the affair must be receive at least one packet,
		//but it not regiest to affair manager.
// 		LOG_ERROR("uid=%d,fatal error:m_bRegiest is FALSE",m_pUserinfo->dwUser_ID);
// 		ASSERT(0);
		return;
	}
	//unRegiester self.
	m_pDvdtAffairMgr->UnRegisterAffair(this);
	m_bRegiest = FALSE;
	//tell dovalidate affairs manager to remove this affair(remove from list ,and free the memory.).
	Remove();
}

VOID DoValidateAffair::EasyVerify( BYTE* pPacket,WORD wSize )
{
	PACKETGENERALHEAD* DvdtHead = (PACKETGENERALHEAD*)pPacket;

	//send buffer.
	BYTE BufferToSend[4096] = {0};
	memset(BufferToSend,0,sizeof(BufferToSend));
	WORD wToSend = 0;
	
	switch(DvdtHead->MesgFullType)
	{
	case NETMESG_LGDVDT_FIRSTKNOCK:

		break;
	case NETMESG_LGDVDT_2ND:
		wToSend = EasyVerifyCheckMacs(pPacket,wSize,BufferToSend,sizeof(BufferToSend));
		if (wToSend)
		{
			Send(BufferToSend,wToSend);
		}else{
			CloseSession();
		}
		break;
	default:
		CloseSession();
		break;
	}
}

//do validate and set m_doValidateStep a appropriate value.
VOID DoValidateAffair::DoValidate( BYTE* pPacket,WORD wSize )
{
	//check size
	if (wSize < sizeof(PACKETGENERALHEAD)){
		LOG_ERROR("[bad-size] wSize < PACKETGENERALHEAD uid = %d,,IP=%s",m_pUserinfo->dwUser_ID,m_szSessionIp);
		return;
	}
}

void DoValidateAffair::SetClientIp( const char* ip )
{
	strncpy(m_szSessionIp,ip,sizeof(m_szSessionIp));
	DebugMessage("####set client ip = %s####",m_szSessionIp);
}

WORD DoValidateAffair::EasyVerifyCheckMacs( const BYTE* pPacket,WORD PacketSize,OUT BYTE* sendBuffer,DWORD dwBufferMaxSize )
{
	return 0;
}

std::map<std::string,unsigned int> DoValidateAffair::genClientFileInfo( BYTE * pMsg, WORD wSize )
{
	std::map<std::string,unsigned int> ret;
	if (wSize - sizeof(PACKETGENERALHEAD) <= 0)
	{
		return ret;
	}

	DWORD contentsLen = wSize - sizeof(PACKETGENERALHEAD);
	if (contentsLen%36 != 0)
	{
		return ret;
	}

	for (int i=0; i!=contentsLen/36; i++)
	{
		BYTE* pb = pMsg + sizeof(PACKETGENERALHEAD) + 36*i;
		char name[32] = {0};
		memcpy(name,pb,32);
		DWORD crc32s = *(DWORD*)(pb+32);
		DebugMessage("genClientFileInfo name=%s,fcrc=0x%08X",name,crc32s);
		ret.insert(make_pair(std::string(name),crc32s));
	}

	return ret;
}