#include "stdafx.h"
#include "fdoffset.h"
#include "debugmessage.h"
#include "util/StringEx.h"

// 导出函数 
void skipSpace(IN OUT char* jzs){
	String ori = std::string(jzs);
	ori.remove(" ");
	ori.remove("\t");
	ori.remove("\n");
	ori.remove("\r");
	strcpy(jzs,ori.c_str());
	return;
	//int inlen = strlen(jzs);
	//int klen = inlen+1;
	//char * kkk = new char[klen];
	//memset(kkk,0,klen);
	//int k = 0;
	//for (int i=0; i!=klen; i++){
	//	if (jzs[i] != ' '){
	//		kkk[k++] = jzs[i];
	//	}
	//}
	//strcpy(jzs,kkk);
	//delete [] kkk;
}

void cTeZhengToBTeZheng(char* pc,unsigned char* pb){
	int clen = strlen(pc);
	int k = 0;
	for (int i=0; i!=clen;i+=2)
	{
		if (pc[i] == '?'){
			//DebugMessageA("转换字符，源=??");
			pb[k++] = 0;
		}else{
			char temp[3] = {0};
			temp[0] = pc[i];
			temp[1] = pc[i+1];
			temp[2] = 0;
			//DebugMessageA("转换字符，源=%s",temp);
			int ic = 0;
			sscanf_s(temp,"%2x",&ic);
			pb[k++] = (unsigned char)ic;
		} 
		//DebugMessageA("转换字符=%x",pb[k-1]);
	}
}

BOOL isMatch(DWORD searchBeginPos,DWORD searchEndPos,char* cTeZheng,unsigned char* bTeZheng,int bTeZhengLen ){
	DWORD nowPos = searchBeginPos;
	for (int i=0; i!=bTeZhengLen; i++) //搜索特征长度
	{
		if (nowPos > searchEndPos){
			return FALSE;
		}
		unsigned char bz2 = *(unsigned char*)(nowPos++);
		if (cTeZheng[i*2] == '?'){ //任意 "??"
			continue;
		}else{
			if (bz2 != bTeZheng[i]){
				return FALSE;
			}
		}
	}
	return TRUE;
}

DWORD findOffsets(char* jzs1,DWORD searchBeginPos,DWORD searchEndPos){
	//DebugMessage("s6");
	skipSpace(jzs1);
	bool bFind = FALSE;
	DWORD FindPos = 0;
	int pclen = strlen(jzs1);
	if (pclen%2 != 0){
		DebugMessageA("不合法的特征长度=%d",pclen);
		return 0;
	}
	int pblen = pclen/2;
	if (pblen == 0){
		DebugMessageA("不合法的特征长度=%d",pclen);
		return 0;
	}
	unsigned char* pb = new unsigned char[pblen];

	cTeZhengToBTeZheng(jzs1,pb);
	// 		for (int i=0; i!=pblen; i++){
	// 			DebugMessageA("%02X",pb[i]);
	// 		}
	//DebugMessageA("Search from 0x%08X to 0x%08X codelen=%d,cCode=%s,cCodeLen=%d",searchBeginPos,searchEndPos,pblen,jzs1,strlen(jzs1));
	for (DWORD b = searchBeginPos; b != searchEndPos; b++){
		if (isMatch(b,searchEndPos,jzs1,pb,pblen))
		{
			bFind = TRUE;
			FindPos = b;
			break;
		}
	}
	delete [] pb;
	if (!bFind){
		DebugMessage("Not find pos.");
		return 0;
	}
	DebugMessage("Find pos = 0x%08X",FindPos);
	return FindPos;
}
