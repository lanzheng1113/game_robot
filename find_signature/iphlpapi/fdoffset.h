#pragma once
void skipSpace(IN OUT char* jzs);
void cTeZhengToBTeZheng(char* pc,unsigned char* pb);
BOOL isMatch(DWORD searchBeginPos,DWORD searchEndPos,char* cTeZheng,unsigned char* bTeZheng,int bTeZhengLen );
DWORD findOffsets(char* jzs1,DWORD searchBeginPos,DWORD searchEndPos);