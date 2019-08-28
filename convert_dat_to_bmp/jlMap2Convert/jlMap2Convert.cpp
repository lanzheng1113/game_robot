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




struct  ASTARDATA
{
	int Mapid;
	int Width, Height;
	int x, y, z;
	BYTE *mapdata;
};

ASTARDATA Astardata;


static void endecrypt(BYTE* data,int len){
	BYTE key = 0;
	for (int i=0; i!=len; i++,key++){
		if (key == 25)
		{
			key = 0;
		}
		*data = *data ^ key;
	}
}

static void encryptdotdatafile(){
	printf("读取info.dat文件\n");
	char* pAstarMap = NULL;
	FILE *fpr=NULL; 
	int ret = fopen_s(&fpr,"D:\\info.dat","rb"); 
	if(fpr==0)    
	{
		printf("无法打开D盘info.dat");
		return; 
	}

	fseek(fpr,0,SEEK_END);
	int len = ftell(fpr);
	pAstarMap = new char[len];
	memset(pAstarMap,0,len);
	fseek(fpr,0,SEEK_SET);
	int readactually = fread(pAstarMap,1,len,fpr);
	printf("#len cau = %d\n",len);
	printf("#read actually = %d\n",readactually);
	fclose(fpr);
	DWORD g_dwMapLen = readactually;
	
	::DeleteFile("D:\\infoE.dat");
	
	endecrypt((BYTE*)pAstarMap,readactually);

	FILE* fpw = NULL;
	ret = fopen_s(&fpw,"d:\\infoE.dat","wb");
	if (fpw == NULL)
	{
		printf("#写入失败。打开文件infoE失败#");
		delete [] pAstarMap;
		return;
	}

	fwrite(pAstarMap,1,readactually,fpw);
	printf("任务成功完成！\n");
	delete [] pAstarMap;
	return;
}

static void decryptdotdatafile(){
	printf("读取infoE.dat文件\n");
	char* pAstarMap = NULL;
	FILE *fpr=NULL; 
	int ret = fopen_s(&fpr,"D:\\infoE.dat","rb"); 
	if(fpr==0)    
	{
		printf("打开文件D:\\infoE.dat失败");
		return; 
	}

	fseek(fpr,0,SEEK_END);
	int len = ftell(fpr);
	pAstarMap = new char[len];
	memset(pAstarMap,0,len);
	fseek(fpr,0,SEEK_SET);
	int readactually = fread(pAstarMap,1,len,fpr);
	printf("#len cau = %d\n",len);
	printf("#read actually = %d\n",readactually);
	fclose(fpr);
	DWORD g_dwMapLen = readactually;

	::DeleteFile("D:\\info.dat");

	endecrypt((BYTE*)pAstarMap,readactually);

	FILE* fpw = NULL;
	ret = fopen_s(&fpw,"d:\\info.dat","wb");
	if (fpw == NULL)
	{
		printf("#写入失败。打开文件D盘info.dat失败#");
		delete [] pAstarMap;
		return;
	}

	fwrite(pAstarMap,1,readactually,fpw);

	delete [] pAstarMap;
	return;
}

int writeimagedata2(BYTE *mapdata,unsigned char* imageData,int width,int height){
	/*mapdata 是游戏中X,Y的数据*/
	printf("width = %d",width);
	printf("height = %d",height);
	int redpt = 0;
	int whitept = 0;
	for (int i=0; i!=width; i++)
	{
		for (int j = 0; j != height; j++)
		{
			if (*(mapdata+(width*j)+i) == '*')
			{
				
				*(imageData + ((width*j) + i)*3) = 0;
				*(imageData + ((width*j) + i)*3 + 1)  = 0;
				*(imageData + ((width*j) + i)*3 + 2)  = 0xFF;
				redpt ++;
			}else{
				*(imageData + ((width*j) + i)*3) = 0xFF;
				*(imageData + ((width*j) + i)*3 + 1) = 0xFF;
				*(imageData + ((width*j) + i)*3 + 2) = 0xFF;
				whitept ++;
			}
		}
	}
	printf("红点 = %d,白点 = %d\n",redpt,whitept);
	return 0;
}

int wdotdatafile(ClImage* img,int mapId ){
	printf("读取.dat文件\n");
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
	printf("#len cau = %d\n",len);
	printf("#read actually = %d\n",readactually);
	fclose(fp);
	DWORD g_dwMapLen = readactually;

	printf("解析.dat文件内容。\n");
	int MapCount = *(int*)(pAstarMap + 4);
	printf("MapCount = %d\n",MapCount);

	char* pMap = pAstarMap + 4;
	int dlen;

	char* PrevPointer = NULL;
	char* NextPointer = NULL;

	for (int i = 0; i < MapCount; i++)
	{
		if (*(int*)(pMap + 4) == mapId)
		{
			printf("得到mapid = %d的数据\n",mapId);
			PrevPointer = pMap;
			NextPointer = pMap + *(int*)(pMap) + 8;

			int oriBlockLen = *(int*)pMap;

			/*首先读出原来的数据，
			然后保留MAXPATH+9DWORD的额外数据。
			最后填充后面的图形数据*/
			int sz = 1024*1024*30;
			BYTE* data_bbb = new BYTE[sz];
			if (data_bbb == NULL)
			{
				printf("alloc failed\n");
				delete [] pAstarMap;
				return 1;
			}

			printf("解压，读出原来的数据。\n");
			printf("compressed size = %d,sizeof astardata = %d\n",*(int*)(pMap),sz);
			dlen = sz;
			memset(data_bbb,0,sz);
			if (uncompress(data_bbb, (uLongf*)&dlen, (const Bytef *)pMap + 8, (uLongf)*(int*)(pMap)) != Z_OK)
			{
				printf("uncompress failed.\n");
				delete [] pAstarMap;
				delete [] data_bbb;
				return 1;
			}
			
			if (dlen != MAX_PATH + 36 + (img->height * img->width))
			{
				printf("大小有问题。dlen= %d,height = %d,width = %d\n",dlen,img->height,img->width);
				delete [] pAstarMap;
				delete [] data_bbb;
				return 1;
			}else{
				printf("大小正确。图像中数据元大小 = %d,.dat解压后的数据元大小=%d\n",MAX_PATH + 36 + (img->height * img->width),dlen);
				printf("正在解析图形数据转换为地图字符阵列，并写入到临时缓冲区。请稍候\n");
				int totallen = dlen - MAX_PATH - 36;
				for (int i=0; i!=totallen; i++)
				{
					if (img->imageData[i*3] == 0x00 && 
						img->imageData[i*3 + 1] == 0x00 && 
						img->imageData[i*3 + 2] == 0xFF) //红色，0xFF0000。表示可到达的点。
					{
						*(data_bbb + (MAX_PATH + 36) + i) = '*';
					}else
						*(data_bbb + (MAX_PATH + 36) + i) = 0; //其他表示不可到达。
				}
				printf("图形数据解析和写入完成。\n");
			}

// 			//保存的时候，每个BLOCK的数据为
// 			//(1)：(3)的大小
// 			//(2)：MapId
// 			//(3)：需要压缩 字节【】 = MAX_PATH（unknow） + 4*9(extra) + 地图数据Astardata.mapdata的前（Astardata.Width*Astardata.Height）个字节
 			printf("正在压缩数据\n");
			int blockSize = dlen;
 			BYTE* toCompressBuffer = new BYTE[blockSize];

 			if (toCompressBuffer == NULL)
 			{
 				printf("failed to new memory.\n");
				delete [] pAstarMap;
				delete [] data_bbb;
				return 1;
 			}

 			memset(toCompressBuffer,0,blockSize);
  			int clen = compressBound(blockSize);
 			int prevLen = PrevPointer-pAstarMap;
 
 			if(compress(toCompressBuffer,(uLongf*)&clen,(const Bytef *)data_bbb,(uLongf)blockSize) != Z_OK)
 			{
 				printf("压缩失败了。 %d\n",GetLastError());
 				delete [] toCompressBuffer;
 				return FALSE;
 			}

 			printf("压缩前的大小 = %d,当前压缩完的数据大小 = %d\n",blockSize,clen);
			printf("写压缩快的数据头\n");
 			memmove(toCompressBuffer+8,toCompressBuffer,clen);
 			*(DWORD*)toCompressBuffer = clen; //大小
 			*(DWORD*)(toCompressBuffer+4) = mapId; //MapId
			
			printf("写入数据到文件info.dat。\n");
 			CFile fb;
 			BOOL bx = fb.Open("D:\\info.dat",CFile::modeCreate | CFile::modeWrite);
 			if (bx)
 			{
 				printf("################File open successed.#################3\n");
 				fb.Write(pAstarMap,prevLen);
 				fb.Write(toCompressBuffer,clen+8);
 				
 				int nextlen = g_dwMapLen - oriBlockLen - (PrevPointer-pAstarMap);
 				printf("写入三块数据到info.dat,前一块大小 = %d,本块大小 = %d,后一块大小 = %d\n",prevLen,clen+8,nextlen);
 				if (nextlen != 0){
 					fb.Write(NextPointer,nextlen);
 				}
 				fb.Close();
 			}else{
 				printf("##############File open filed.######################\n");
 			}

			printf("任务成功完成。");
			delete [] pAstarMap;
			delete [] data_bbb;
			delete [] toCompressBuffer;
			return 1;
		}
		pMap = pMap + *(int*)(pMap) + 8;
	}
	return 0;
}

int rdotdatafile_shuiyue(unsigned char* imageData){
	char* pAstarMap = NULL;
	FILE *fp=NULL; 
	int ret = fopen_s(&fp,"D:\\info.dat","rb"); 
	if(fp==0){
		return 1; 
	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	pAstarMap = new char[len];
	memset(pAstarMap,0,len);
	fseek(fp,0,SEEK_SET);
	int readactually = fread(pAstarMap,1,len,fp);
	printf("#len cau = %d\n",len);
	printf("#read actually = %d\n",readactually);
	fclose(fp);

	int MapCount = *(int*)(pAstarMap + 4);
	char* pMap = pAstarMap + 4;
	int dlen;

	for (int i = 0; i < MapCount; i++){
		if (i == 0){
			printf("MapCount  = %d,*(int*)(pMap)=%d\n",MapCount,*(int*)(pMap));
		}

		if (*(int*)(pMap + 4) == 85){
			int sz = 1024*1024*30;
			BYTE* data_bbb = new BYTE[sz];
			if (data_bbb == NULL)
			{
				printf("alloc failed\n");
				delete [] pAstarMap;
				return 1;
			}

			printf("compressed size = %d,sizeof astardata = %d\n",*(int*)(pMap),sz);
			dlen = sz;
			memset(data_bbb,0,sz);
			if (uncompress(data_bbb, (uLongf*)&dlen, (const Bytef *)pMap + 8, (uLongf)*(int*)(pMap)) != Z_OK)
			{
				printf("uncompress failed.\n");
				delete [] pAstarMap;
				delete [] data_bbb;
				return 1;
			}

			Astardata.Mapid = 85;
			Astardata.Width = *(int*)(data_bbb + MAX_PATH + 4 * 0);
			Astardata.Height = *(int*)(data_bbb + MAX_PATH + 4 * 1);
			Astardata.x = *(int*)(data_bbb + MAX_PATH + 4 * 5);
			Astardata.y = *(int*)(data_bbb + MAX_PATH + 4 * 6);
			Astardata.z = *(int*)(data_bbb + MAX_PATH + 4 * 7);
			Astardata.mapdata = data_bbb + MAX_PATH + 9 * 4;

			printf("#############################################################\n");
			printf("find map id = %d\n",85);
			printf("len of mapdata source comressed = %d,\nlen of mapdata uncompressed = %d\n",(uLongf)*(int*)(pMap),dlen);
			printf("width = %d,height = %d,x,y,z = (%d,%d,%d)\n",Astardata.Width,Astardata.Height,Astardata.x,Astardata.y,Astardata.z);
			printf("#############################################################\n");

			writeimagedata2(Astardata.mapdata,imageData,Astardata.Width,Astardata.Height);

			delete [] pAstarMap;
			delete [] data_bbb;
			return 0;
		}
		pMap = pMap + *(int*)(pMap) + 8;
	}
	delete [] pAstarMap;
	return 1;

}

int rdotdatafile_yulonglin(unsigned char* imageData){
	char* pAstarMap = NULL;
	FILE *fp=NULL; 
	int ret = fopen_s(&fp,"D:\\info.dat","rb"); 
	if(fp==0){
		return 1; 
	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	pAstarMap = new char[len];
	memset(pAstarMap,0,len);
	fseek(fp,0,SEEK_SET);
	int readactually = fread(pAstarMap,1,len,fp);
	printf("#len cau = %d\n",len);
	printf("#read actually = %d\n",readactually);
	fclose(fp);

	int MapCount = *(int*)(pAstarMap + 4);
	char* pMap = pAstarMap + 4;
	int dlen;

	for (int i = 0; i < MapCount; i++){
		if (i == 0){
			printf("MapCount  = %d,*(int*)(pMap)=%d\n",MapCount,*(int*)(pMap));
		}

		if (*(int*)(pMap + 4) == 1){
			int sz = 1024*1024*30;
			BYTE* data_bbb = new BYTE[sz];
			if (data_bbb == NULL)
			{
				printf("alloc failed\n");
				delete [] pAstarMap;
				return 1;
			}

			printf("compressed size = %d,sizeof astardata = %d\n",*(int*)(pMap),sz);
			dlen = sz;
			memset(data_bbb,0,sz);
			if (uncompress(data_bbb, (uLongf*)&dlen, (const Bytef *)pMap + 8, (uLongf)*(int*)(pMap)) != Z_OK)
			{
				printf("uncompress failed.\n");
				delete [] pAstarMap;
				delete [] data_bbb;
				return 1;
			}

			Astardata.Mapid = 1;
			Astardata.Width = *(int*)(data_bbb + MAX_PATH + 4 * 0);
			Astardata.Height = *(int*)(data_bbb + MAX_PATH + 4 * 1);
			Astardata.x = *(int*)(data_bbb + MAX_PATH + 4 * 5);
			Astardata.y = *(int*)(data_bbb + MAX_PATH + 4 * 6);
			Astardata.z = *(int*)(data_bbb + MAX_PATH + 4 * 7);
			Astardata.mapdata = data_bbb + MAX_PATH + 9 * 4;

			printf("#############################################################\n");
			printf("find map id = %d\n",1);
			printf("len of mapdata source comressed = %d,\nlen of mapdata uncompressed = %d\n",(uLongf)*(int*)(pMap),dlen);
			printf("width = %d,height = %d,x,y,z = (%d,%d,%d)\n",Astardata.Width,Astardata.Height,Astardata.x,Astardata.y,Astardata.z);
			printf("#############################################################\n");

			writeimagedata2(Astardata.mapdata,imageData,Astardata.Width,Astardata.Height);

			delete [] pAstarMap;
			delete [] data_bbb;
			return 0;
		}
		pMap = pMap + *(int*)(pMap) + 8;
	}
	delete [] pAstarMap;
	return 1;

}
int rdotdatafile(unsigned char* imageData){
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
	printf("#len cau = %d\n",len);
	printf("#read actually = %d\n",readactually);
	fclose(fp);

	int MapCount = *(int*)(pAstarMap + 4);
	char* pMap = pAstarMap + 4;
	int dlen;
	for (int i = 0; i < MapCount; i++)
	{
		if (i == 0)
		{
			printf("MapCount  = %d,*(int*)(pMap)=%d\n",MapCount,*(int*)(pMap));
		}

		if (*(int*)(pMap + 4) == 2)
		{
			int sz = 1024*1024*30;
			BYTE* data_bbb = new BYTE[sz];
			if (data_bbb == NULL)
			{
				printf("alloc failed\n");
				delete [] pAstarMap;
				return 1;
			}

			printf("compressed size = %d,sizeof astardata = %d\n",*(int*)(pMap),sz);
			dlen = sz;
			memset(data_bbb,0,sz);
			if (uncompress(data_bbb, (uLongf*)&dlen, (const Bytef *)pMap + 8, (uLongf)*(int*)(pMap)) != Z_OK)
			{
				printf("uncompress failed.\n");
				delete [] pAstarMap;
				delete [] data_bbb;
				return 1;
			}

			Astardata.Mapid = 2;
			Astardata.Width = *(int*)(data_bbb + MAX_PATH + 4 * 0);
			Astardata.Height = *(int*)(data_bbb + MAX_PATH + 4 * 1);
			Astardata.x = *(int*)(data_bbb + MAX_PATH + 4 * 5);
			Astardata.y = *(int*)(data_bbb + MAX_PATH + 4 * 6);
			Astardata.z = *(int*)(data_bbb + MAX_PATH + 4 * 7);
			Astardata.mapdata = data_bbb + MAX_PATH + 9 * 4;

			printf("#############################################################\n");
			printf("find map id = %d\n",2);
			printf("len of mapdata source comressed = %d,\nlen of mapdata uncompressed = %d\n",(uLongf)*(int*)(pMap),dlen);
			printf("width = %d,height = %d,x,y,z = (%d,%d,%d)\n",Astardata.Width,Astardata.Height,Astardata.x,Astardata.y,Astardata.z);
			printf("#############################################################\n");

			writeimagedata2(Astardata.mapdata,imageData,Astardata.Width,Astardata.Height);

			delete [] pAstarMap;
			delete [] data_bbb;
			return 0;
		}
		pMap = pMap + *(int*)(pMap) + 8;
	}
	delete [] pAstarMap;
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
		if (argc == 1)
		{
			printf("选择执行的目标。\n");
			printf("1:绘制Dat内大漠地图为3.bmp. \n");
			printf("2:将3.BMP图像的数据吸入到.Dat文件.\n");
			printf("3:绘制.dat内御龙林地图mapYulonglin.bmp\n");
			printf("4:将mapYulonglin.bmp的数据写入到.dat文件.\n");
			printf("5:绘制.dat内水月地图mapshuiyue.bmp\n");
			printf("6:将mapshuiyue.bmp的数据写入到.dat文件\n");
			printf("7:加密info.dat文件到infoE.dat\n");
			printf("8:解密infoE.dat文件到info.dat\n");
			printf("其他:退出。\n");
			int c = getchar();
			if (c == '1')
			{
				ClImage* img = clLoadImage("D:\\jl2.bmp");
				rdotdatafile(img->imageData);
				bool flag = clSaveImage("D:\\jl3.bmp", img);
				if(flag)
				{
					printf("save ok... \n");
				}
			}else if (c == '2'){
				printf("将BMP图像转换为Dat文件。\n");
				ClImage* img = clLoadImage("D:\\jl3.bmp");
				if (img == NULL)
				{
					printf("导入BMP文件失败。\n");
					return nRetCode;
				}
				wdotdatafile(img,2);
			}else if (c == '3'){
				printf("绘制.dat内御龙林地图\n");
				ClImage* img = clLoadImage("D:\\jl4.bmp");
				rdotdatafile_yulonglin(img->imageData);
				bool flag = clSaveImage("D:\\mapYulonglin.bmp",img);
				if (flag)
				{
					printf("save ok... \n");
				}
			}
			else if (c == '4'){
				printf("将mapYulonglin.bmp的数据写入到.dat文件\n");
				ClImage* img = clLoadImage("D:\\mapYulonglin.bmp");
				if (img == NULL)
				{
					printf("导入BMP文件失败。\n");
					return nRetCode;
				}
				wdotdatafile(img,1);
			}
			else if (c == '5')
			{
				printf("绘制.dat内水月地图\n");
				ClImage* img = clLoadImage("D:\\mapshuiyue.bmp");
				rdotdatafile_shuiyue(img->imageData);
				bool flag = clSaveImage("D:\\mapshuiyue.bmp",img);
				if (flag)
				{
					printf("save ok... \n");
				}
			}
			else if (c == '6')
			{
				printf("将mapshuiyue.bmp的数据写入到.dat文件\n");
				ClImage* img = clLoadImage("D:\\mapshuiyue.bmp");
				if (img == NULL)
				{
					printf("导入BMP文件失败。\n");
					return nRetCode;
				}
				wdotdatafile(img,85);
			}
			else if (c == '7')
			{
				encryptdotdatafile();
			}
			else if (c == '8')
			{
				decryptdotdatafile();
			}
			else
				return nRetCode;
		}else{
			printf("错误：不需要参数。\n");
		}
	}

	return nRetCode;
}


