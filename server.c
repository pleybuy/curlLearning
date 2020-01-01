//
// Created by pleybuy on 2019/12/27.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "make_log.h"

//libevent http server header files
#include <event2/http.h>
#include <event2/event.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include <include/cJSON.h>

void generic_cb(struct evhttp_request* req, void* arg)
{
    char* s = "This is the generic buf";
    evbuffer_add(req->output_buffer, s, strlen(s));
    evhttp_send_reply(req, 200, "OK", NULL);
}

void test_cb(struct evhttp_request* req, void* arg)
{
    const char* uri = evhttp_request_get_uri(req);
    /*判断req是不是get请求*/
    if(evhttp_request_get_command(req) == EVHTTP_REQ_GET){
        struct evbuffer* buf = evbuffer_new();
        if(buf == NULL) {
            evhttp_send_error(req, HTTP_NOTFOUND, "server error\n");
            return;
        }
        evbuffer_add_printf(buf, "Requested: %s\n", uri);
        //HTTP header
        evhttp_add_header(evhttp_request_get_output_headers(req), "Server", "MoCarHttpd v0.1");
        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain; charset=UTF-8");
        evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "close");
        evhttp_send_reply(req, HTTP_OK, "OK", buf);
        printf("get uri:%s\n", uri);
        LOG("server","test_cb", "get uri:%s\n", uri);
        return;
    }
    //如果不是post请求  直接返回 200 OK
    if(evhttp_request_get_command(req) != EVHTTP_REQ_POST){
        evhttp_send_reply(req, 200, "OK", NULL);
        return;
    }
    printf("Got a POST request for <%s>\n", uri);
    LOG("server","test_cb", "Got a POST request for <%s>\n", uri);

    //判断URI是否合法
    struct evhttp_uri *decoded = evhttp_uri_parse(uri);
    if(!decoded){
        printf("Illegal URI. \n");
        return;
    }
    /* Decode the payload */
    struct evbuffer *buf = evhttp_request_get_input_buffer(req);
    evbuffer_add(buf,"",1);
    char *payload = (char *)evbuffer_pullup(buf, -1);
    int post_data_len = evbuffer_get_length(buf);
    char request_data_buf[4096] = {0};
    memcpy(request_data_buf, payload, post_data_len);
    printf("[post_data][%d] = \n%s\n", post_data_len, payload);
    //unpack json
    cJSON* root = cJSON_Parse(request_data_buf);
    cJSON* name = cJSON_GetObjectItem(root, "name");
    cJSON* age = cJSON_GetObjectItem(root, "age");
    printf("================\n name = %s\n age = %d \n===================", name->valuestring, age->valueint);


    //todo some work to do with unpacked json data
    cJSON_Delete(root);
    //回包
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "result", "OK");
    cJSON_AddStringToObject(root, "sessionID", "11dssdsd49074534");
    char* res = cJSON_Print(root);

    //HTTP header
    evhttp_add_header(evhttp_request_get_output_headers(req), "Server", "libeventHttpd v0.1");
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "close");
    char tmpBuf[10] = {0};
    sprintf(tmpBuf, "%d", HTTP_OK);
    evhttp_add_header(evhttp_request_get_output_headers(req), "status", tmpBuf);
    struct evbuffer *evb = evbuffer_new();
    evbuffer_add_printf(evb, "%s", res);
    //将封装好的evbuffer 发送给客户端
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
}

int main()
{
    short http_port = 8081;
    char* http_addr = "0.0.0.0";

    struct event_base* base = event_base_new();
    struct evhttp* http_server = evhttp_new(base);
    if(NULL == http_server)
    {
        return -1;
    }

    int ret = evhttp_bind_socket(http_server, http_addr, http_port);
    if(ret != 0)
    {
        return -1;
    }
    evhttp_set_cb(http_server, "/test", test_cb, "arg");
    //没有注册过的URI都会走到这里
    evhttp_set_gencb(http_server, generic_cb, NULL);
    printf("http server start OK!\n");


    /* 开始阻塞监听 (永久执行) */
    event_base_dispatch(base);
    evhttp_free(http_server);

    return 0;
}

