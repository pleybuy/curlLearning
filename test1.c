/*************************************************************************
	> File Name: test1.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Wed 11 Dec 2019 08:21:29 PM CST
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "include/cJSON.h"
#include "make_log.h"

typedef struct response_data{
    char data[4096];
    size_t data_len;
}response_data_t;
/*
 *ptr 表示收到服务器返回数据的首地址
 *size 表示返回数据的大小
 *nmemb 表示返回数据的个数
 *userdata用户给回调函数传递的形参
 */
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *responseData)
{
    response_data_t *rsp = (response_data_t *)responseData;
    int count = size * nmemb;//数据的长度
    memcpy(rsp->data, ptr, count);
    rsp->data_len = count;
    return rsp->data_len;
}

char *getJsonData(){
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "hutao");
    cJSON_AddNumberToObject(root, "age", 18);
    char *res = cJSON_Print(root);
    cJSON_Delete(root);
    return res;
}

int main()
{
    //接收返回的数据
    response_data_t responseData;
    memset(&responseData, 0, sizeof(response_data_t));
	//1 创建一个curl句柄
	CURL *curl = NULL;
	CURLcode res;
	//2. 初始化一个句柄
	curl = curl_easy_init();
	//3 给该句柄设定一些参数
	curl_easy_setopt(curl, CURLOPT_URL , "http://172.17.4.10:8081/test");

    //4 变成post请求
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    //5 设置需要传递的post数据
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getJsonData());

	//6 处理服务器响应结果 给当前句柄设置一个处理从服务器返回数据的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    //给回调函数传递一个形参
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    //7 将curl句柄向远程服务器提交请求
    res = curl_easy_perform(curl);
    if(res == CURLE_OK){
        printf("curl easy perform success res = %d\n",res);
        curl_easy_cleanup(curl);

        printf("============================================\n");
        //7.处理返回的数据
        LOG("client", "responseData", responseData.data);
        printf("%s\n", responseData.data);
        printf("\n============================================\n");
        cJSON* root = cJSON_Parse(responseData.data);
        cJSON* result = cJSON_GetObjectItem(root, "result");
        cJSON* sID = cJSON_GetObjectItem(root, "sessionID");
        printf("result = %s\n", result->valuestring);
        printf("sessionID = %s\n", sID->valuestring);
        cJSON_Delete(root);
        return 0;
    }else {
        printf("curl easy perform error, res = %d\n", res);
        //6 执行错误需要 释放句柄空间
        curl_easy_cleanup(curl);
        return -1;
    }
}
