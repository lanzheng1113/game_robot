#include "stdafx.h"
#include "CSend.h"
#include "winsock2.h"
#include "StringEx.h"
using std::string;
#include "AutoLock.h"

static CLock_CS ccs;

int CSend( IN SOCKET s, const char FAR * buf, IN int len )
{
	CAutoLock lock(ccs);
	char* my_buf = new char[len+2];
	*(WORD*)my_buf = len;
	memcpy(my_buf+2,buf,len);

	int actlen = len + 2;

	int sentCount = 0;
	int bufsizeRemain = actlen;
	printf("%s begin:\n",__FUNCTION__);
	while (sentCount != actlen )
	{
		int isend = send(s,my_buf+sentCount,bufsizeRemain,0);
		if (isend == SOCKET_ERROR || isend == 0) {
			DWORD dw = GetLastError();
			printf("send error with last error:%d",dw);

			string str = "因与服务器的通讯发生错误：错误ID=";
			str += String::fromLong(dw);
			str += "，启用操作失败！请重试！";

			printf("%s",str.c_str());
			delete [] my_buf;
			return isend;
		}
		sentCount += isend;
		bufsizeRemain -= isend;
	}

	if (sentCount == actlen)//send success
		sentCount = len;
	else
		printf("send count error!");

	delete [] my_buf;
	printf("%s end:\n",__FUNCTION__);
	return sentCount;
}
