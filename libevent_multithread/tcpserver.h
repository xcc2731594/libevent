#ifndef TCPSERVER_H
#define TCPSERVER_H


#include "thread.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event2/listener.h>

//db
// #include "database.h"

class Tcpserver
{
private:
    Thread *m_threadPool;
    int m_threadNum;
    struct event_base *m_base;
    struct evconnlistener* m_listener;
    int currentThread;
//db
    // Database *m_db;

public:
    Tcpserver(int threadNum=2);
    ~Tcpserver();
    int listen(int port,const char* ip=NULL);
    void start();
    void listenEvent(evutil_socket_t fd,struct sockaddr_in* clientAddr);

//虚函数
    virtual void readEvent(bufferevent* bev);
    virtual void writeEvent();
    virtual void closeEvent();
    virtual void connectEvent();

protected:
    static void listener_cb(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr* sa,int socklen,void* user_data);
    static void connect_readcb(bufferevent* bev,void* user_data);
    static void event_callback(bufferevent* bev,short events,void* user_data);

};



#endif