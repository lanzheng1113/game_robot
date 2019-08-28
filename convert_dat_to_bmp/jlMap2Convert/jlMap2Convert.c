// jlMap2Convert.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zlib.lib")

#include "jlMap2Convert.h"
#include "chenLeeCV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;


// int _tmain(int argc, _TCHAR* argv[])
// {
// 	ClImage* img = clLoadImage("c:/test.bmp");
// 	bool flag = clSaveImage("c:/result.bmp", img);
// 	if(flag)
// 	{
// 		printf("save ok... \n");
// 	}
// 
// 	while(1);
// 	return 0;
// }
// 
// void bmpRead(){
// 	FILE *fp=NULL; 
// 	int ret = fopen_s(&fp,"D:\\jl2.bmp","rb"); 
// 	if(fp==0)    
// 	{
// 		return ; 
// 	}
// 
// 	BITMAPFILEHEADER fileheader={0}; 
// 	fread(&fileheader,sizeof(fileheader),1,fp); 
// 	if(fileheader.bfType!=0x4D42) 
// 	{ 
// 		fclose(fp); 
// 		return ; 
// 	} 
// 
// 	BITMAPINFOHEADER head; 
// 	fread(&head,sizeof(BITMAPINFOHEADER),1,fp);  
// 
// 	long bmpWidth = head.biWidth; 
// 	printf("bmp width = %d\n",bmpWidth);
// 	long bmpHeight = head.biHeight; 
// 	printf("bmp height = %d\n",bmpHeight);
// 	WORD biBitCount = head.biBitCount; 
// 	printf("bmp count = %d\n",biBitCount);
// 	if(biBitCount != 24) 
// 	{ 
// 		::AfxMessageBox(_T("请选择24位位图！")); 
// 		fclose(fp); 
// 		return ; 
// 	} 
// 
// 	int totalSize = (bmpWidth *biBitCount/8+3)/4*4*bmpHeight; 
// 	printf("total size = %d\n",totalSize);
// 
// 	BYTE *pBmpBuf = new BYTE[totalSize]; 
// 	size_t size = 0; 
// 	while(true) 
// 	{ 
// 		int iret = fread(&pBmpBuf[size],1,1,fp); 
// 		if(iret == 0) 
// 			break; 
// 		size = size + iret; 
// 	} 
// 	fclose(fp); 
// 
// 
// 
//  	int i,j; 
// // 	CClientDC dc(this); 
//  	int pitch = bmpWidth%4; 
//  	for(i=bmpHeight;i>0;i--) 
//  	{ 
//  		int realPitch=i*pitch; 
//  		for(j=0;j<bmpWidth;j++) 
//  		{ 
// // 			dc.SetPixel(j,i,
// // 				RGB( pBmpBuf[(i*bmpWidth+j)*3+2+realPitch], pBmpBuf[(i*bmpWidth+j)*3+1+realPitch], pBmpBuf[(i*bmpWidth+j)*3+realPitch])); 
//  		} 
//  	} 
// 	delete [] pBmpBuf;     
// 	pBmpBuf = NULL;
// 	return ; 
// }
// 
// void setPix(BYTE* pixBuf,int pixHeight,int x,int y, char BlockFlag)
// {
// 	//左下是第一点，右上是最后一点。
// 	//24位图像，需要3个字节。
// 	BYTE* writePos = pixBuf + (pixHeight-y)*x*4;
// 	if (BlockFlag == '*')
// 	{
// 		*(COLORREF*)writePos = RGB(0xFF,0,0);
// 	}else{
// 		*(COLORREF*)writePos = RGB(0,0,0);
// 	}
// 	return;
// }

struct  ASTARDATA
{
	int Mapid;
	int Width, Height;
	int x, y, z;
	BYTE data[1024 * 1024 * 100];
	BYTE *mapdata;
};

ASTARDATA Astardata;

int rdotdatafile(){
	char* pAstarMap = NULL;
 	FILE *fp=NULL; 
 	int ret = fopen_s(&fp,"D:\\info.dat","rb"); 
 	if(fp==0)    
 	{
 		return 1; 
 	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	pAstarMap = new char[len];
	memset(pAstarMap,0,len);
	fseek(fp,0,SEEK_SET);
	int readactually = fread(pAstarMap,1,len,fp);
	
	printf("len cau = %d",len);
	printf("read actually = %d",readactually);
	fclose(fp);

	int MapCount = *(int*)(pAstarMap + 4);
	char* pMap = pAstarMap + 4;
	int dlen;
	for (int i = 0; i < MapCount; i++)
	{
		if (i == 0)
		{
			printf("MapCount  = %d,*(int*)(pMap)=%d",MapCount,*(int*)(pMap));
		}

		if (*(int*)(pMap + 4) == 2)
		{

			if (uncompress(Astardata.data, (uLongf*)&dlen, (const Bytef *)pMap + 8, (uLongf)*(int*)(pMap)) != Z_OK)
			{
				return 1;
			}

			Astardata.Mapid = 2;
			Astardata.Width = *(int*)(Astardata.data + MAX_PATH + 4 * 0);
			Astardata.Height = *(int*)(Astardata.data + MAX_PATH + 4 * 1);
			Astardata.x = *(int*)(Astardata.data + MAX_PATH + 4 * 5);
			Astardata.y = *(int*)(Astardata.data + MAX_PATH + 4 * 6);
			Astardata.z = *(int*)(Astardata.data + MAX_PATH + 4 * 7);
			Astardata.mapdata = Astardata.data + MAX_PATH + 9 * 4;

			printf("#############################################################\n");
			printf("find map id = %d",2);
			printf("len of mapdata source comressed = %d,len of mapdata uncompressed = %d",(uLongf)*(int*)(pMap),dlen);
			printf("width = %d,height = %d,x,y,z = (%d,%d,%d)",Astardata.Width,Astardata.Height,Astardata.x,Astardata.y,Astardata.z);
			printf("#############################################################\n");

			return 0;
		}
		pMap = pMap + *(int*)(pMap) + 8;
	}
	return 1;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		ClImage* img = clLoadImage("D:\\jl2.bmp");
		rdotdatafile();
		bool flag = clSaveImage("D:\\jl3.bmp", img);
		if(flag)
		{
			printf("save ok... \n");
		}
	}

	return nRetCode;
}


