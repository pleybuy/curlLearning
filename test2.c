/*************************************************************************
	> File Name: test2.c
	> Author: pleybuy
	> Mail: 1149074534@qq.com 
	> Created Time: Wed 11 Dec 2019 09:26:03 PM CST
 ************************************************************************/

#include<stdio.h>
#include <event2/event.h>
int main(){
    struct event_base* base = event_base_new();
    if(!base){
        printf("event base init error\n");
        return -1;
    }


	

	return 0;
	

}

