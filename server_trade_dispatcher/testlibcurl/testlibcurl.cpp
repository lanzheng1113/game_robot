// testlibcurl.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "curl/include/curl/curl.h"
#include <stdio.h>
#include <string.h>
/**
 * @File
 * this is a test project for libcurl
 *
 */

CURL *curl;
CURLcode res;

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (strlen((char *)stream) + strlen((char *)ptr) > 100000-1)
		return 0;
	strcat((char*)stream, (char *)ptr);
	return size*nmemb;
}

char *down_file(char *filename)
{
	static char str[100000];
	strcpy(str,"");
	//return ��<a href=/��http://gtk.awaysoft.com//��>��;
	curl_easy_setopt(curl, CURLOPT_URL, filename); //�������ص�ַ
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);//���ó�ʱʱ��
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//����д���ݵĺ���
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);//����д���ݵı���
	res = curl_easy_perform(curl);//ִ������
	str[100000-1] = NULL;
	if(CURLE_OK != res) 
		return NULL;//�ж��Ƿ����سɹ�
	return str;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char url[200]="http://baike.baidu.com/view/1806203.htm";
	curl = curl_easy_init();//��curl���г�ʼ��
	char *result;
	while(fgets(url, 200, stdin)){
		result = down_file(url);
		if (result) puts(result);
		else puts("Get Error!");
		printf("/nPlease Input a url:");

	}
	curl_easy_cleanup(curl);//�ͷ�curl��Դ

	return 0;
}

