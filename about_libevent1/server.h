#ifndef SERVER_H
#define SERVER_H
#include <event.h>
#include <event2/listener.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include "chatinfo.h"
#include <jsoncpp/json/json.h>
class Server
{
private:
    struct event_base *base; //事件集合
    struct evconnlistener *listener;  //监听事件
    ChatInfo *chatlist; //连表对象
    ChatDataBase *chatdb;//数据库对象
    void server_register(struct bufferevent *bev,Json::Value val);
    
public:
    Server(const char*ip="127.0.0.1",int port=8000);
    ~Server();
    static void listener_cb(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr *addr,int socklen,void *arg);
    static void client_handler(int fd,void *);
    static void read_cb(struct bufferevent *bev,void *ctx);
    static void event_cb(struct bufferevent *bev,short what,void *ctx);

    
};

#endif