/*************************************************************************
	> File Name: test1.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Wed 11 Dec 2019 08:21:29 PM CST
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
int main1()
{
	//1 创建一个curl句柄
	CURL *curl = NULL;
	CURLcode res;
	//2. 初始化一个句柄
	curl = curl_easy_init();
	//3 给该句柄设定一些参数
	curl_easy_setopt(curl, CURLOPT_URL , "http://www.baidu,com");
	//4 将curl句柄向远程服务器提交请求 
	res = curl_easy_perform(curl);
	if(res == CURLE_OK){
		printf("curl easy perform error res = %d\n",res);
		curl_easy_cleanup(curl);
		return 1;
	}
	//5 处理服务器响应结果
	
	//6 释放句柄空间
	curl_easy_cleanup(curl);
	
	return 0;
}
