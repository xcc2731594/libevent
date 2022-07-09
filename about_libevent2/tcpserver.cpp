#include "tcpserver.h"
Tcpserver::Tcpserver(int threadNum)
{
    //初始化m_base
    m_base = event_base_new();
    if(!m_base)
    {
        printf("could not create an event_base");
        exit(1);
    }
    if(0>=threadNum)
    {
        printf("threadNum error");
        exit(1);
    }
    //初始化线程池
    m_threadNum = threadNum;
    m_threadPool = new Thread[m_threadNum];
    currentThread = 0;
}

Tcpserver::~Tcpserver()
{
}

int Tcpserver::listen(int port,const char *ip)
{
    sockaddr_in sin;
    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    if(NULL!=ip)
    {
        inet_aton(ip,&sin.sin_addr);
    }
    m_listener = evconnlistener_new_bind(m_base,listener_cb,this,
    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,
    (struct sockaddr*)&sin,sizeof(sin));
    if(!m_listener)
    {
        printf("could not create a listener\n");
        return 1;
    }
    //开启线程池
    for(int i=0;i<m_threadNum;i++)
    {
        m_threadPool[i].start();
        printf("线程%d开始运行\n",i+1);
    }
    printf("done\n");
    return 0;


}

void Tcpserver::listener_cb(struct evconnlistener *,evutil_socket_t fd,struct sockaddr* clientAddr,int,void*data)
{
    printf("有客户端请求连接\n");
    Tcpserver *p = (Tcpserver*) data;
    p->listenEvent(fd,(struct sockaddr_in*)clientAddr);
}

void Tcpserver::start()
{
    //开始监听
    event_base_dispatch(m_base);
    evconnlistener_free(m_listener);
    event_base_free(m_base);
}

void Tcpserver::listenEvent(evutil_socket_t fd,struct sockaddr_in *clientAddr)
{
    //线程的轮询操作
    char *ip = inet_ntoa(clientAddr->sin_addr);
    uint16_t port = ntohs(clientAddr->sin_port);

    struct event_base *base = m_threadPool[currentThread].get_base();
    printf("线程:%d开始监听客户端%s\n",(int)m_threadPool[currentThread].get_threadId(),ip);
    currentThread = (currentThread+1)%m_threadNum;
    struct bufferevent *bev;
    bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    if(!bev)
    {
        printf("error constructing bufferevent!");
        event_base_loopbreak(base);
        return ;
    }

}