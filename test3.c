/*************************************************************************
	> File Name: test3.c
	> Author: pleybuy
	> Mail: 1149074534@qq.com 
	> Created Time: Thu 12 Dec 2019 10:28:00 AM CST
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define POSTDATA "{\"username\":\"ace\", \"passwd\":\"123123\", \"isDriver\":\"yes\"}"
/*
 *ptr 表示收到服务器返回数据的首地址
 *size 表示返回数据的大小
 *nmemb表示返回数据的个数
 *userdata用户给回调函数传递的形参
 */
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE*fp = (FILE*)userdata;
	int count = size * nmemb;//数据的长度
	int	fwrite_len = fwrite(ptr, size, nmemb, fp);
	printf("读取的总长度%d\n", count);
	printf("写入的总长度%d\n", fwrite_len);
}

int main3()
{
	FILE* fp = NULL;
	//创建句柄
	CURL *curl = NULL;
	CURLcode res;

	curl = curl_easy_init();
	if(curl == NULL){
		printf("crul == NULL\n");
		return -1;
	}
	fp = fopen("file.txt", "w");


	//设定一些参数
	curl_easy_setopt(curl, CURLOPT_URL, "http://172.17.4.10:7777/");

	//给当前curl变成post请求
	curl_easy_setopt(curl, CURLOPT_POST, 1);

	//给当前curl设置需要传递的post数据
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTDATA);
	
	//给当前句柄设置一个处理从服务器返回数据的回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	//给回调函数传递一个形参
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	res = curl_easy_perform(curl);
	if(res != CURLE_OK){
		printf("perform error res = %d\n", res);
		return -1;
	}
	return 0;

}

