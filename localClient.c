/*************************************************************************
	> File Name: localClient.c
	> Author: pleybuy
	> Mail: 1149074534@qq.com 
	> Created Time: Mon 30 Dec 2019 01:12:01 PM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
int main(){
    int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd == -1){
        printf("socket error\n");
        return -1;
    }
    unlink("client.socket");
    struct sockaddr_un client;
    client.sun_family = AF_LOCAL;
    strcpy(client.sun_path, "client.socket");

    int ret = bind(fd, (struct sockaddr*)&client, sizeof(client));
    if(ret == -1){
        perror("bind error");
        exit(-2);
    }
    //初始化server信息
    struct sockaddr_un serv;
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "server.socket");

    //连接服务器
    ret = connect(fd, (struct sockaddr*)&serv, sizeof(serv));
    if(ret == -1){
        perror("connect server error");
        return -3;
    }

    //通信
    while (1){
        char buf[1024] = {0};
        fgets(buf, sizeof(buf), stdin);
        send(fd, buf, strlen(buf) +1 , 0);

        //接受数据
        recv(fd, buf, sizeof(buf), 0);
        printf("buf = %s\n", buf);
    }
    //close(fd);
	return 0;
}

