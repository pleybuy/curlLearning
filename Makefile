cc=gcc
CPPFLAGS= -I. -I./include -I/usr/local/include/event2/
CFLAGS=-Wall
LIBS=-lpthread -levent -lcurl -lrt -lm
#找到当前目录下所有的.c文件
src = $(wildcard *.c)
#将当前目录下所有的.c 转换成.o给obj
obj = $(patsubst %.c, %.o, $(src))

test_curl=./test_curl
server=./server
libeventTest=./libeventTest
localClient=./localClient

target=$(test_curl) $(server) $(libeventTest) $(localClient)
ALL:$(target)

#生成所有的.o文件
$(obj):%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

#server程序
$(server):server.o cJSON.o make_log.o
	$(cc) $^ -o $@ $(LIBS)

#test_curl程序
$(test_curl):curlClient.o cJSON.o make_log.o
	$(cc) $^ -o $@ $(LIBS)

#libevent测试程序
$(libeventTest):test2.o
	$(cc) $^ -o $@ $(LIBS)

#localClient
$(localClient):localClient.o
    $(cc) $^ -o $@

#clean
clean:
	-rm -rf $(obj) $(target)

#将clean目标  改成一个虚拟符号
.PHONY:clean ALL
