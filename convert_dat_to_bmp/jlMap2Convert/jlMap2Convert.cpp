// jlMap2Convert.cpp : �������̨Ӧ�ó������ڵ㡣
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


// Ψһ��Ӧ�ó������

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
	printf("��ȡinfo.dat�ļ�\n");
	char* pAstarMap = NULL;
	FILE *fpr=NULL; 
	int ret = fopen_s(&fpr,"D:\\info.dat","rb"); 
	if(fpr==0)    
	{
		printf("�޷���D��info.dat");
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
		printf("#д��ʧ�ܡ����ļ�infoEʧ��#");
		delete [] pAstarMap;
		return;
	}

	fwrite(pAstarMap,1,readactually,fpw);
	printf("����ɹ���ɣ�\n");
	delete [] pAstarMap;
	return;
}

static void decryptdotdatafile(){
	printf("��ȡinfoE.dat�ļ�\n");
	char* pAstarMap = NULL;
	FILE *fpr=NULL; 
	int ret = fopen_s(&fpr,"D:\\infoE.dat","rb"); 
	if(fpr==0)    
	{
		printf("���ļ�D:\\infoE.datʧ��");
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
		printf("#д��ʧ�ܡ����ļ�D��info.datʧ��#");
		delete [] pAstarMap;
		return;
	}

	fwrite(pAstarMap,1,readactually,fpw);

	delete [] pAstarMap;
	return;
}

int writeimagedata2(BYTE *mapdata,unsigned char* imageData,int width,int height){
	/*mapdata ����Ϸ��X,Y������*/
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
	printf("��� = %d,�׵� = %d\n",redpt,whitept);
	return 0;
}

int wdotdatafile(ClImage* img,int mapId ){
	printf("��ȡ.dat�ļ�\n");
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

	printf("����.dat�ļ����ݡ�\n");
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
			printf("�õ�mapid = %d������\n",mapId);
			PrevPointer = pMap;
			NextPointer = pMap + *(int*)(pMap) + 8;

			int oriBlockLen = *(int*)pMap;

			/*���ȶ���ԭ�������ݣ�
			Ȼ����MAXPATH+9DWORD�Ķ������ݡ�
			����������ͼ������*/
			int sz = 1024*1024*30;
			BYTE* data_bbb = new BYTE[sz];
			if (data_bbb == NULL)
			{
				printf("alloc failed\n");
				delete [] pAstarMap;
				return 1;
			}

			printf("��ѹ������ԭ�������ݡ�\n");
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
				printf("��С�����⡣dlen= %d,height = %d,width = %d\n",dlen,img->height,img->width);
				delete [] pAstarMap;
				delete [] data_bbb;
				return 1;
			}else{
				printf("��С��ȷ��ͼ��������Ԫ��С = %d,.dat��ѹ�������Ԫ��С=%d\n",MAX_PATH + 36 + (img->height * img->width),dlen);
				printf("���ڽ���ͼ������ת��Ϊ��ͼ�ַ����У���д�뵽��ʱ�����������Ժ�\n");
				int totallen = dlen - MAX_PATH - 36;
				for (int i=0; i!=totallen; i++)
				{
					if (img->imageData[i*3] == 0x00 && 
						img->imageData[i*3 + 1] == 0x00 && 
						img->imageData[i*3 + 2] == 0xFF) //��ɫ��0xFF0000����ʾ�ɵ���ĵ㡣
					{
						*(data_bbb + (MAX_PATH + 36) + i) = '*';
					}else
						*(data_bbb + (MAX_PATH + 36) + i) = 0; //������ʾ���ɵ��
				}
				printf("ͼ�����ݽ�����д����ɡ�\n");
			}

// 			//�����ʱ��ÿ��BLOCK������Ϊ
// 			//(1)��(3)�Ĵ�С
// 			//(2)��MapId
// 			//(3)����Ҫѹ�� �ֽڡ��� = MAX_PATH��unknow�� + 4*9(extra) + ��ͼ����Astardata.mapdata��ǰ��Astardata.Width*Astardata.Height�����ֽ�
 			printf("����ѹ������\n");
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
 				printf("ѹ��ʧ���ˡ� %d\n",GetLastError());
 				delete [] toCompressBuffer;
 				return FALSE;
 			}

 			printf("ѹ��ǰ�Ĵ�С = %d,��ǰѹ��������ݴ�С = %d\n",blockSize,clen);
			printf("дѹ���������ͷ\n");
 			memmove(toCompressBuffer+8,toCompressBuffer,clen);
 			*(DWORD*)toCompressBuffer = clen; //��С
 			*(DWORD*)(toCompressBuffer+4) = mapId; //MapId
			
			printf("д�����ݵ��ļ�info.dat��\n");
 			CFile fb;
 			BOOL bx = fb.Open("D:\\info.dat",CFile::modeCreate | CFile::modeWrite);
 			if (bx)
 			{
 				printf("################File open successed.#################3\n");
 				fb.Write(pAstarMap,prevLen);
 				fb.Write(toCompressBuffer,clen+8);
 				
 				int nextlen = g_dwMapLen - oriBlockLen - (PrevPointer-pAstarMap);
 				printf("д���������ݵ�info.dat,ǰһ���С = %d,�����С = %d,��һ���С = %d\n",prevLen,clen+8,nextlen);
 				if (nextlen != 0){
 					fb.Write(NextPointer,nextlen);
 				}
 				fb.Close();
 			}else{
 				printf("##############File open filed.######################\n");
 			}

			printf("����ɹ���ɡ�");
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

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		if (argc == 1)
		{
			printf("ѡ��ִ�е�Ŀ�ꡣ\n");
			printf("1:����Dat�ڴ�Į��ͼΪ3.bmp. \n");
			printf("2:��3.BMPͼ����������뵽.Dat�ļ�.\n");
			printf("3:����.dat�������ֵ�ͼmapYulonglin.bmp\n");
			printf("4:��mapYulonglin.bmp������д�뵽.dat�ļ�.\n");
			printf("5:����.dat��ˮ�µ�ͼmapshuiyue.bmp\n");
			printf("6:��mapshuiyue.bmp������д�뵽.dat�ļ�\n");
			printf("7:����info.dat�ļ���infoE.dat\n");
			printf("8:����infoE.dat�ļ���info.dat\n");
			printf("����:�˳���\n");
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
				printf("��BMPͼ��ת��ΪDat�ļ���\n");
				ClImage* img = clLoadImage("D:\\jl3.bmp");
				if (img == NULL)
				{
					printf("����BMP�ļ�ʧ�ܡ�\n");
					return nRetCode;
				}
				wdotdatafile(img,2);
			}else if (c == '3'){
				printf("����.dat�������ֵ�ͼ\n");
				ClImage* img = clLoadImage("D:\\jl4.bmp");
				rdotdatafile_yulonglin(img->imageData);
				bool flag = clSaveImage("D:\\mapYulonglin.bmp",img);
				if (flag)
				{
					printf("save ok... \n");
				}
			}
			else if (c == '4'){
				printf("��mapYulonglin.bmp������д�뵽.dat�ļ�\n");
				ClImage* img = clLoadImage("D:\\mapYulonglin.bmp");
				if (img == NULL)
				{
					printf("����BMP�ļ�ʧ�ܡ�\n");
					return nRetCode;
				}
				wdotdatafile(img,1);
			}
			else if (c == '5')
			{
				printf("����.dat��ˮ�µ�ͼ\n");
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
				printf("��mapshuiyue.bmp������д�뵽.dat�ļ�\n");
				ClImage* img = clLoadImage("D:\\mapshuiyue.bmp");
				if (img == NULL)
				{
					printf("����BMP�ļ�ʧ�ܡ�\n");
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
			printf("���󣺲���Ҫ������\n");
		}
	}

	return nRetCode;
}


