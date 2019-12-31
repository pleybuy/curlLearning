/*************************************************************************
	> File Name: test3.c
	> Author: pleybuy
	> Mail: 1149074534@qq.com 
	> Created Time: Thu 12 Dec 2019 10:28:00 AM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

int main()
{
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(lfd == -1){
        printf("lsock init error, fd = %d\n", lfd);
    }
    unlink("server.socket");
    struct sockaddr_un serv;
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "server.socket");
    //socklen_t len = 0;
    //绑定套接字
    int ret = bind(lfd, (struct sockaddr*)&serv, sizeof(serv));
    //判断是否绑定成功

    //监听
    ret = listen(lfd, 36);

    //等待接受连接请求
    struct sockaddr_un client;
    socklen_t len = 0;

    int cfd = accept(lfd, (struct sockaddr*)&serv, &len);
    if(cfd == -1){
        printf("accept err\n");
        return -1;
    }
    printf("=========client bind file %s===========\n", client.sun_path);

    //通信
    while (1){
        char buf[1024] = {0};
        int recvLen = recv(cfd, buf, sizeof(buf), 0);
        if(recvLen < 0){
            perror("recv error \n");
        }else if(recvLen == 0){
            perror(" client disconnect\n");
        } else{
            printf("recv buf = %s\n", buf);
        }
    }

	return 0;

}

