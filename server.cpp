//
// Created by pleybuy on 2019/12/27.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//libevent http server header files
#include <event2/http.h>
#include <event2/event.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>

void generic_cb(struct evhttp_request* req, void* arg)
{
    char* s = "This is the generic buf";
    evbuffer_add(req->output_buffer, s, strlen(s));
    evhttp_send_reply(req, 200, "OK", NULL);
}

void test_cb(struct evhttp_request* req, void* arg)
{
    char* s = "This is the test223232 buf";
    //evbuffer_add(req->output_buffer, s, strlen(s));
    //HTTP header
    evhttp_add_header(evhttp_request_get_output_headers(req), "Server", "MoCarHttpd v0.1");
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "close");
    struct evbuffer *evb = evbuffer_new();
    evbuffer_add_printf(evb, "%s", s);
    //将封装好的evbuffer 发送给客户端
    evhttp_send_reply(req, 200, "OK", NULL);
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

