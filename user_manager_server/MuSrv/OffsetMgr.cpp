#include "stdafx.h"
#include "OffsetMgr.h"
#include "util/path.h"
#include "DebugMessage.h"
#include "OffsetIndex/OffsetIndexSoul.h"
//BASE OFFSET
/*old
DWORD gOfs_MapObjectBase = 0x000585D0; 
DWORD g_objHero_offset = 0x11620BC;  
DWORD g_objPlayerSet = 0x11620A0;
DWORD ItemsAddresOffset=0x15FC;
*/
DWORD gOfs_MapObjectBase = 0x000585d0; 
DWORD g_objHero_offset = 0xb5a3b4;  
DWORD g_objPlayerSet = 0xb5a398;
DWORD ItemsAddresOffset=0x15FC;
DWORD eyeX=0;
DWORD eyeY=0;
DWORD PackageOffset=0;
DWORD PetOffset = 0;
DWORD MyAccOffset = 0;
DWORD g_CDOffset = 0;
DWORD g_CD2Offset = 0;
DWORD g_RoleStateOffset = 0;
DWORD g_objNetworkOffset = 0;
DWORD g_BooththisOffset = 0;
DWORD g_CreateHero3Offset = 0;
DWORD g_CreateHeroObjOffset = 0;
DWORD g_CreateHeroObj2Offset = 0;
DWORD g_CallCreateHeroOffset = 0;
DWORD g_SelectedVersionCAll = 0;
DWORD g_SelectedSmallAreaCAll = 0;
DWORD g_SelectLargeBigAreaCAll = 0;
DWORD g_LoginCall = 0;
DWORD g_SimFlashobj = 0;
DWORD g_AccountObj = 0;
DWORD g_PasswordObj = 0;
DWORD g_PasswordTableObj = 0;
DWORD g_loginobjOffset = 0;
DWORD g_SetPasswordTableCALl = 0;
DWORD g_MyChangeDateCALlOffset = 0;
DWORD g_GuoLvShangYe = 0;
DWORD g_GuoLvLoginMessageBox = 0;
DWORD g_CloseCurWndFunOffset = 0;
DWORD g_CloseCurWndOffset = 0;
DWORD g_SpeedUpOffset = 0;
DWORD g_OffsetDingWeiLiaoTian = 0;
DWORD g_OffsetPanDuanFengHao = 0;
DWORD g_callSetHwndText = 0;
DWORD g_OfsMsgWndContent = 0;
/*
g_objPlayerSet=0x11620A0
g_pObjHero=0x11620BC
MapBase=585D0
*/

DWORD OffsetMgr::genBaiTanOffset( DWORD* pOffset,WORD count/*=40*/ ){
	pOffset[0] = gOfs_MapObjectBase;
	pOffset[1] = g_objHero_offset;
	pOffset[2] = g_objPlayerSet;
	pOffset[3] = 0;
	pOffset[4] = 0;
	pOffset[5] = 0;
	pOffset[6] = PackageOffset;
	pOffset[7] = 0;
	srand(GetTickCount());
	for (int i=8; i!=count; i++){
		pOffset[i] = rand();
	}
	return 0;
}

DWORD OffsetMgr::genTrueOffset( DWORD* pOffset,WORD count/*=40*/ )
{
	if (count < index_end+1)
	{
		return 0;
	}

	pOffset[index_offsetMapBase] = gOfs_MapObjectBase;
	pOffset[index_OffsetGetObjHere] = g_objHero_offset;
	pOffset[index_OffsetGetObjPlayerSet] = g_objPlayerSet;
	pOffset[index_ItemsAddresOffset] = ItemsAddresOffset;
	pOffset[index_offsetEyeX] = eyeX;
	pOffset[index_offsetEyeY] = eyeY;
	pOffset[index_PackageOffset] = PackageOffset;
	pOffset[index_PetEnageOffset] = PetOffset;
	pOffset[index_szAccountOffset] = MyAccOffset;
	pOffset[index_cd1Offset] = g_CDOffset;
	pOffset[index_cd2Offset] = g_CD2Offset;
	pOffset[index_RoleState] = g_RoleStateOffset;
	pOffset[index_OffsetObjNetwork] = g_objNetworkOffset;
	pOffset[index_BooththisOffset] = g_BooththisOffset;
	pOffset[index_CreateHero3Offset] = g_CreateHero3Offset;
	pOffset[index_CreateHeroObjOffset] = g_CreateHeroObjOffset;
	pOffset[index_CreateHeroObj2Offset] = g_CreateHeroObj2Offset;
	pOffset[index_CallCreateHeroOffset] = g_CallCreateHeroOffset;
	pOffset[index_SelectedVersionCAll] = g_SelectedVersionCAll;
	pOffset[index_SelectedSmallAreaCAll] = g_SelectedSmallAreaCAll;
	pOffset[index_SelectLargeBigAreaCAll] = g_SelectLargeBigAreaCAll;
	pOffset[index_LoginCall] = g_LoginCall;
	pOffset[index_SimFlashobj] = g_SimFlashobj;
	pOffset[index_AccountObj] = g_AccountObj;
	pOffset[index_PasswordObj] = g_PasswordObj;
	pOffset[index_PasswordTableObj] = g_PasswordTableObj;
	pOffset[index_loginobjOffset] = g_loginobjOffset;
	pOffset[index_SetPasswordTableCALl] = g_SetPasswordTableCALl;
	pOffset[index_MyChangeDateCALlOffset] = g_MyChangeDateCALlOffset;
	pOffset[index_GuoLvShangYe] = g_GuoLvShangYe;
	pOffset[index_GuoLvLoginMessageBox] = g_GuoLvLoginMessageBox;
	pOffset[index_CloseCurWndFunOffset] = g_CloseCurWndFunOffset;
	pOffset[index_CloseCurWndOffset] = g_CloseCurWndOffset;
	pOffset[index_SpeedUpOffset] = g_SpeedUpOffset;
	pOffset[index_OffsetDingWeiLiaoTian] = g_OffsetDingWeiLiaoTian;
	pOffset[index_OffsetPanDuanFengHao] = g_OffsetPanDuanFengHao;
	pOffset[index_callSetHwndText] = g_callSetHwndText;
	pOffset[index_offsetGameWndMessageContent] = g_OfsMsgWndContent;
	pOffset[index_end] = 0;

	srand(GetTickCount());

	for (int i=index_end; i< count; i++){
		pOffset[i] = rand();
	}

	DWORD seed = pOffset[count-1];
	unsigned char* pEncrypt =  (unsigned char*)pOffset;

	unsigned char encryptb1 = (unsigned char)(seed & 0x000000ff);
	unsigned char encryptb2 = (unsigned char)((seed & 0x0000ff00) >> 8);
	unsigned char encryptb3 = (unsigned char)((seed & 0x00ff0000) >> 16);
	unsigned char encryptb4 = (unsigned char)((seed & 0xff000000) >> 24);
	const int cryptBytes = 4*(count-1);

	for (int i = 0; i!=cryptBytes; i++)
	{
		pEncrypt[i] = pEncrypt[i] ^ encryptb1;
	}
	for (int i = 0; i!=cryptBytes; i++)
	{
		pEncrypt[i] = pEncrypt[i] ^ encryptb2;
	}
	for (int i = 0; i!=cryptBytes; i++)
	{
		pEncrypt[i] = pEncrypt[i] ^ encryptb3;
	}
	for (int i = 0; i!=cryptBytes; i++)
	{
		pEncrypt[i] = pEncrypt[i] ^ encryptb4;
	}

	return 0;
}

DWORD OffsetMgr::genFalseOffset( DWORD* pOffset,WORD count/*=40*/ ){
	pOffset[0] = gOfs_MapObjectBase;
	pOffset[1] = g_objHero_offset;
	pOffset[2] = g_objPlayerSet;
	srand(GetTickCount());
	for (int i=3; i!=count; i++){
		pOffset[i] = rand();
	}
	return 0;
}

void OffsetMgr::reloadOffsetFromInis()
{
	std::string iniPath = Path::getApplicationDirPath();
	iniPath += "MyOffset.ini";
	char szBuffer[256] = {0};
	GetPrivateProfileString("offset","gOfs_MapObjectBase","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&gOfs_MapObjectBase);

	GetPrivateProfileString("offset","g_objHero_offset","0",szBuffer,sizeof(szBuffer),iniPath.c_str()); 
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_objHero_offset);

	GetPrivateProfileString("offset","g_objPlayerSet","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_objPlayerSet);

	GetPrivateProfileString("offset","ItemsAddresOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&ItemsAddresOffset);

	GetPrivateProfileString("offset","eyeX","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&eyeX);

	GetPrivateProfileString("offset","eyeY","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&eyeY);

	GetPrivateProfileString("offset","PackageOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&PackageOffset);

	GetPrivateProfileString("offset","宠物体力","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&PetOffset);

	GetPrivateProfileString("offset","账号偏移","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&MyAccOffset);

	GetPrivateProfileString("offset","CD","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CDOffset);

	GetPrivateProfileString("offset","CD2","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CD2Offset);

	GetPrivateProfileString("offset","人物状态","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_RoleStateOffset);

	GetPrivateProfileString("offset","网络对象","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_objNetworkOffset);

	GetPrivateProfileString("offset","BooththisOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_BooththisOffset);

	GetPrivateProfileString("offset","CreateHero3Offset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CreateHero3Offset);
	
	GetPrivateProfileString("offset","CreateHeroObjOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CreateHeroObjOffset);

	GetPrivateProfileString("offset","CreateHeroObj2Offset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CreateHeroObj2Offset);

	GetPrivateProfileString("offset","CallCreateHeroOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CallCreateHeroOffset);

	GetPrivateProfileString("offset","SelectedVersionCAll","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_SelectedVersionCAll);

	GetPrivateProfileString("offset","SelectedSmallAreaCAll","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_SelectedSmallAreaCAll);

	GetPrivateProfileString("offset","SelectLargeBigAreaCAll","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_SelectLargeBigAreaCAll);

	GetPrivateProfileString("offset","LoginCall","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_LoginCall);

	GetPrivateProfileString("offset","SimFlashobj","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_SimFlashobj);

	GetPrivateProfileString("offset","AccountObj","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_AccountObj);

	GetPrivateProfileString("offset","PasswordObj","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_PasswordObj);

	GetPrivateProfileString("offset","PasswordTableObj","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_PasswordTableObj);

	GetPrivateProfileString("offset","loginobjOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_loginobjOffset);

	GetPrivateProfileString("offset","SetPasswordTableCALl","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_SetPasswordTableCALl);

	GetPrivateProfileString("offset","MyChangeDateCALlOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_MyChangeDateCALlOffset);

	GetPrivateProfileString("offset","GuoLvShangYe","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_GuoLvShangYe);

	GetPrivateProfileString("offset","GuoLvLoginMessageBox","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_GuoLvLoginMessageBox);

	GetPrivateProfileString("offset","CloseCurWndFunOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CloseCurWndFunOffset);

	GetPrivateProfileString("offset","CloseCurWndOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_CloseCurWndOffset);

	GetPrivateProfileString("offset","SpeedUpOffset","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_SpeedUpOffset);

	GetPrivateProfileString("offset","OffsetDingWeiLiaoTian","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_OffsetDingWeiLiaoTian);

	GetPrivateProfileString("offset","OffsetPanDuanFengHao","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_OffsetPanDuanFengHao);

	GetPrivateProfileString("offset","callSetHwndText","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_callSetHwndText);

	GetPrivateProfileString("offset","MsgWndContent","0",szBuffer,sizeof(szBuffer),iniPath.c_str());
	StrToIntEx(szBuffer,STIF_SUPPORT_HEX,(int*)&g_OfsMsgWndContent);

	//g_RoleStateOffset
	DebugMessageA("######After rload######");
	DebugMessageA("gOfs_MapObjectBase=0x%08X",gOfs_MapObjectBase);
	DebugMessageA("g_objHero_offset=0x%08X",g_objHero_offset);
	DebugMessageA("g_objPlayerSet=0x%08X",g_objPlayerSet);
	DebugMessageA("ItemsAddresOffset=0x%08X",ItemsAddresOffset);
	DebugMessageA("eyeX=0x%08X",eyeX);
	DebugMessageA("eyeY=0x%08X",eyeY);
	DebugMessageA("PackageOffset=0x%08X",PackageOffset);
	DebugMessageA("宠物体力OFFSET=0x%08X",PetOffset);
	DebugMessageA("账号偏移=0x%08X",MyAccOffset);
	DebugMessageA("CD=0x%08X",g_CDOffset);
	DebugMessageA("CD2 = 0x%08X",g_CD2Offset);
	DebugMessageA("人物状态 = 0x%08X",g_RoleStateOffset);
	DebugMessageA("网络对象 = 0x%08X",g_objNetworkOffset);
	DebugMessageA("OffsetPanDuanFengHao = 0x%08X",g_OffsetPanDuanFengHao);
	DebugMessageA("OffsetDingWeiLiaoTian = 0x%08X",g_OffsetDingWeiLiaoTian);
	DebugMessageA("SpeedUpOffset = 0x%08X",g_SpeedUpOffset);
	DebugMessageA("CloseCurWndOffset = 0x%08X",g_CloseCurWndOffset);
	DebugMessageA("CloseCurWndFunOffset = 0x%08X",g_CloseCurWndFunOffset);
	DebugMessageA("GuoLvLoginMessageBox = 0x%08X",g_GuoLvLoginMessageBox);
	DebugMessageA("GuoLvShangYe = 0x%08X",g_GuoLvShangYe);
	DebugMessageA("MyChangeDateCALlOffset = 0x%08X",g_MyChangeDateCALlOffset);
	DebugMessageA("SetPasswordTableCALl = 0x%08X",g_SetPasswordTableCALl);
	DebugMessageA("loginobjOffset = 0x%08X",g_loginobjOffset);
	DebugMessageA("PasswordTableObj = 0x%08X",g_PasswordTableObj);
	DebugMessageA("PasswordObj = 0x%08X",g_PasswordObj);
	DebugMessageA("AccountObj = 0x%08X",g_AccountObj);
	DebugMessageA("SimFlashobj = 0x%08X",g_SimFlashobj);
	DebugMessageA("LoginCall = 0x%08X",g_LoginCall);
	DebugMessageA("SelectLargeBigAreaCAll = 0x%08X",g_SelectLargeBigAreaCAll);
	DebugMessageA("SelectedSmallAreaCAll = 0x%08X",g_SelectedSmallAreaCAll);
	DebugMessageA("SelectedVersionCAll = 0x%08X",g_SelectedVersionCAll);
	DebugMessageA("CallCreateHeroOffset = 0x%08X",g_CallCreateHeroOffset);
	DebugMessageA("CreateHeroObj2Offset = 0x%08X",g_CreateHeroObj2Offset);
	DebugMessageA("CreateHeroObjOffset = 0x%08X",g_CreateHeroObjOffset);
	DebugMessageA("CreateHero3Offset = 0x%08X",g_CreateHero3Offset);
	DebugMessageA("BooththisOffset = 0x%08X",g_BooththisOffset);
	DebugMessageA("callSetHwndText = 0x%08X",g_callSetHwndText);
	DebugMessageA("g_OfsMsgWndContent = 0x%08X",g_OfsMsgWndContent);
	//g_OfsMsgWndContent
	DebugMessageA("######dump rload end######");
}

void OffsetMgr::remoteUpdate( DWORD* pOffset,WORD count )
{
	DebugMessageA("Dump 收到的偏移！");
	for (int i=0; i!=index_end; i++)
	{
		DebugMessageA("pOffset[%d] = 0x%X",i,pOffset[i]);
	}
	gOfs_MapObjectBase = pOffset[index_offsetMapBase];
	g_objHero_offset = pOffset[index_OffsetGetObjHere];
	g_objPlayerSet = pOffset[index_OffsetGetObjPlayerSet];
	ItemsAddresOffset = pOffset[index_ItemsAddresOffset];
	eyeX = pOffset[index_offsetEyeX];
	eyeY = pOffset[index_offsetEyeY];
	PackageOffset = pOffset[index_PackageOffset];
	PetOffset = pOffset[index_PetEnageOffset];
	MyAccOffset = pOffset[index_szAccountOffset];
	g_CDOffset = pOffset[index_cd1Offset];
	g_CD2Offset = pOffset[index_cd2Offset];
	g_RoleStateOffset = pOffset[index_RoleState];
	g_objNetworkOffset = pOffset[index_OffsetObjNetwork];
	g_BooththisOffset = pOffset[index_BooththisOffset];
	g_CreateHero3Offset = pOffset[index_CreateHero3Offset];
	g_CreateHeroObjOffset = pOffset[index_CreateHeroObjOffset];
	g_CreateHeroObj2Offset = pOffset[index_CreateHeroObj2Offset];
	g_CallCreateHeroOffset = pOffset[index_CallCreateHeroOffset];
	g_SelectedVersionCAll = pOffset[index_SelectedVersionCAll];
	g_SelectedSmallAreaCAll = pOffset[index_SelectedSmallAreaCAll];
	g_SelectLargeBigAreaCAll = pOffset[index_SelectLargeBigAreaCAll];
	g_LoginCall = pOffset[index_LoginCall];
	g_SimFlashobj = pOffset[index_SimFlashobj];
	g_AccountObj = pOffset[index_AccountObj];
	g_PasswordObj = pOffset[index_PasswordObj];
	g_PasswordTableObj = pOffset[index_PasswordTableObj];
	g_loginobjOffset = pOffset[index_loginobjOffset];
	g_SetPasswordTableCALl = pOffset[index_SetPasswordTableCALl];
	g_MyChangeDateCALlOffset = pOffset[index_MyChangeDateCALlOffset];
	g_GuoLvShangYe = pOffset[index_GuoLvShangYe];
	g_GuoLvLoginMessageBox = pOffset[index_GuoLvLoginMessageBox];
	g_CloseCurWndFunOffset = pOffset[index_CloseCurWndFunOffset];
	g_CloseCurWndOffset = pOffset[index_CloseCurWndOffset];
	g_SpeedUpOffset = pOffset[index_SpeedUpOffset];
	g_OffsetDingWeiLiaoTian = pOffset[index_OffsetDingWeiLiaoTian];
	g_OffsetPanDuanFengHao = pOffset[index_OffsetPanDuanFengHao];
	g_callSetHwndText = pOffset[index_callSetHwndText];
	g_OfsMsgWndContent = pOffset[index_offsetGameWndMessageContent];
	std::string iniPath = Path::getApplicationDirPath();
	iniPath += "MyOffset.ini";
	char szBuffer[256] = {0};
	sprintf(szBuffer,"0x%X",gOfs_MapObjectBase);
	WritePrivateProfileString("offset","gOfs_MapObjectBase",szBuffer,iniPath.c_str());
	
	sprintf(szBuffer,"0x%X",g_objHero_offset);
	WritePrivateProfileString("offset","g_objHero_offset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_objPlayerSet);
	WritePrivateProfileString("offset","g_objPlayerSet",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",ItemsAddresOffset);
	WritePrivateProfileString("offset","ItemsAddresOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",eyeX);
	WritePrivateProfileString("offset","eyeX",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",eyeY);
	WritePrivateProfileString("offset","eyeY",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",PackageOffset);
	WritePrivateProfileString("offset","PackageOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",PetOffset);
	WritePrivateProfileString("offset","宠物体力",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",MyAccOffset);
	WritePrivateProfileString("offset","账号偏移",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_CDOffset);
	WritePrivateProfileString("offset","CD",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_CD2Offset);
	WritePrivateProfileString("offset","CD2",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_RoleStateOffset);
	WritePrivateProfileString("offset","人物状态",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_objNetworkOffset);
	WritePrivateProfileString("offset","网络对象",szBuffer,iniPath.c_str());
	/**/
	
	sprintf(szBuffer,"0x%X",g_BooththisOffset);
	WritePrivateProfileString("offset","BooththisOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_CreateHero3Offset);
	WritePrivateProfileString("offset","CreateHero3Offset",szBuffer,iniPath.c_str());
	
	sprintf(szBuffer,"0x%X",g_CreateHeroObjOffset);
	WritePrivateProfileString("offset","CreateHeroObjOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_CreateHeroObj2Offset);
	WritePrivateProfileString("offset","CreateHeroObj2Offset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_CallCreateHeroOffset);
	WritePrivateProfileString("offset","CallCreateHeroOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_SelectedVersionCAll);
	WritePrivateProfileString("offset","SelectedVersionCAll",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_SelectedSmallAreaCAll);
	WritePrivateProfileString("offset","SelectedSmallAreaCAll",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_SelectLargeBigAreaCAll);
	WritePrivateProfileString("offset","SelectLargeBigAreaCAll",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_LoginCall);
	WritePrivateProfileString("offset","LoginCall",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_SimFlashobj);
	WritePrivateProfileString("offset","SimFlashobj",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_AccountObj);
	WritePrivateProfileString("offset","AccountObj",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_PasswordObj);
	WritePrivateProfileString("offset","PasswordObj",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_PasswordTableObj);
	WritePrivateProfileString("offset","PasswordTableObj",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_loginobjOffset);
	WritePrivateProfileString("offset","loginobjOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_SetPasswordTableCALl);
	WritePrivateProfileString("offset","SetPasswordTableCALl",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_MyChangeDateCALlOffset);
	WritePrivateProfileString("offset","MyChangeDateCALlOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_GuoLvShangYe);
	WritePrivateProfileString("offset","GuoLvShangYe",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_GuoLvLoginMessageBox);
	WritePrivateProfileString("offset","GuoLvLoginMessageBox",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_CloseCurWndFunOffset);
	WritePrivateProfileString("offset","CloseCurWndFunOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_CloseCurWndOffset);
	WritePrivateProfileString("offset","CloseCurWndOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_SpeedUpOffset);
	WritePrivateProfileString("offset","SpeedUpOffset",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_OffsetDingWeiLiaoTian);
	WritePrivateProfileString("offset","OffsetDingWeiLiaoTian",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_OffsetPanDuanFengHao);
	WritePrivateProfileString("offset","OffsetPanDuanFengHao",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_callSetHwndText);
	WritePrivateProfileString("offset","callSetHwndText",szBuffer,iniPath.c_str());

	sprintf(szBuffer,"0x%X",g_OfsMsgWndContent);
	WritePrivateProfileString("offset","MsgWndContent",szBuffer,iniPath.c_str());

	DebugMessageA("更新完毕！");
}

