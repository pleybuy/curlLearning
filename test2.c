/*************************************************************************
	> File Name: test2.c
	> Author: pleybuy
	> Mail: 1149074534@qq.com 
	> Created Time: Wed 11 Dec 2019 09:26:03 PM CST
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define POSTDATA "{\"username\":\"ace\", \"pwd\":\"123123\",\"type\":\"10\"}"
int main2(){
	CURL* curl = NULL;
	CURLcode res;
	curl = curl_easy_init();
	if(curl == NULL){
		printf("curl easy init error\n");
		return 1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, "http://172.17.4.10:7777");
	//变成post请求
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	
	//设置需要传递的post数据
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTDATA);

	//提交curl
	res = curl_easy_perform(curl);
	if(res != CURLE_OK){
		printf("perform error res = %d\n", res);
		return 1;
	}
	//处理服务器的返回数据
	
	//清理
	curl_easy_cleanup(curl);

	

	return 0;
	

}

