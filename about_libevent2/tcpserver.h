#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "thread.h"
#include <event.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Tcpserver
{
private:
    Thread *m_threadPool; //线程池
    int m_threadNum; //线程池的大小
    event_base *m_base; //事件集合
    evconnlistener *m_listener; //监听事件
    int currentThread;
public:
    Tcpserver(int threadNum=8);
    ~Tcpserver();
    int listen(int port,const char *ip=NULL); //表示允许监听所有ip
    void start();    //服务器的运行函数
    void listenEvent(evutil_socket_t fd,struct sockaddr_in *clientAddr);//客户端连接的处理函数
protected:
    //客户端连接的回调函数，有客户端连接就会调用
    static void listener_cb(struct evconnlistener *,evutil_socket_t,struct sockaddr*,int,void*);
};




































#endif