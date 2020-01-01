/*************************************************************************
	> File Name: test1.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Wed 11 Dec 2019 08:21:29 PM CST
 ************************************************************************/

#include <stdio.h>
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
    //printf("size = %d, nmemb = %d\n", (int)size, (int)nmemb);
    response_data_t *rsp = (response_data_t *)responseData;
    int count = size * nmemb;//数据的长度
    memcpy(rsp->data + rsp->data_len, ptr, count);
    rsp->data_len += count;
    return count;//这边返回值一定要是写入的大小！！！
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
    response_data_t headData;
    memset(&responseData, 0, sizeof(response_data_t));
    memset(&headData, 0, sizeof(response_data_t));
	//1 创建一个curl句柄
	CURL *curl = NULL;
	CURLcode res;
	int ret = 0;
	res = curl_global_init(CURL_GLOBAL_ALL);
	if(res != 0){
	    printf("curl_global_init error, res = %d\n", res);
        return -1;
	}
	//2. 打印curl当前的版本
	char *cVersion = curl_version();
	printf("curl version = %s\n", cVersion);

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
    //设置回调函数的最后一个用户指针参数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    //7. 设置头部回调函数，一旦接收到http头部数据就调用该函数
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headData);
    //curl_easy_setopt(curl, CURLOPT_HEADER, 1);

    //8. 设置连接等待时间和传输等待时间
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);

    //9 消息头设置
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Hey-server-hey: how are you?");
    headers = curl_slist_append(headers, "X-silly-content: yes");
    /* pass our list of custom made headers */
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    //7 将curl句柄向远程服务器提交请求
    res = curl_easy_perform(curl);
    if(res == CURLE_OK){
        printf("curl easy perform success res = %d\n",res);
        printf("============================================\n");
        //7.处理返回的数据
        LOG("client", "responseData", responseData.data);
        //printf("body = %s\n", responseData.data);
        printf("headLen = %d\n", (int)headData.data_len);
        printf("head = %s", headData.data);
        printf("============================================\n");
        cJSON* root = cJSON_Parse(responseData.data);
        cJSON* result = cJSON_GetObjectItem(root, "result");
        cJSON* sID = cJSON_GetObjectItem(root, "sessionID");
        printf("result = %s\n", result->valuestring);
        printf("sessionID = %s\n", sID->valuestring);
        cJSON_Delete(root);
        return 0;
    }else {
        //取得错误原因字符串 const char *curl_easy_strerror(CURLcode errornum )
        const char *strRea = curl_easy_strerror(res);
        printf("curl easy perform error, res = %d\n", res);
        printf("error reason = %s\n", strRea);
        ret = -1;
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    curl_slist_free_all(headers);
    return ret;
}
