#include "tcpserver.h"

Tcpserver::Tcpserver(int threadNum)
{
    m_base = event_base_new();
    if(!m_base)
    {
        printf("could not create");
        exit(1);
    }
    if(0>=threadNum)
    {
        exit(1);
    }
    m_threadNum = threadNum;
    m_threadPool = new Thread[m_threadNum];

    //db
    // m_db = new Database(NULL,"root","root","test");
    // Json::Value outjson;
    // char*sql = "select * from user";
    // m_db->database_select(sql,outjson);
    // printf("%s\n",outjson["name"][0].toStyledString().c_str());
    // std::string s = outjson.toStyledString();
    // printf("----%s\n",s.c_str());

}

Tcpserver::~Tcpserver()
{

}
int Tcpserver::listen(int port,const char* ip)
{
    struct sockaddr_in sin;
    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    if(NULL!=ip)
    {
        inet_aton(ip,&(sin.sin_addr));
    }
    m_listener = evconnlistener_new_bind(m_base,listener_cb,this,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,(struct sockaddr*)&sin,sizeof(sin));
    
    if(!m_listener)
    {
        return 1;
    }
    for(int i=0;i<m_threadNum;i++)
    {
        m_threadPool[i].start();
        // printf("thread%drunning\n",m_threadPool[i].get_threadId());
    }

    printf("done\n");
    return 0;
}

void Tcpserver::listener_cb(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr* clientaddr,int socklen,void* user_data)
{
    printf("connect coming;\n");
    Tcpserver *p = (Tcpserver*)user_data;
    p->listenEvent(fd,(struct sockaddr_in*)clientaddr);
    // printf("+++++");
    
}

void Tcpserver::start()
{
    event_base_dispatch(m_base);
    evconnlistener_free(m_listener);
    event_base_free(m_base);
}


void Tcpserver::listenEvent(evutil_socket_t fd,struct sockaddr_in* clientAddr)
{
    char* ip = inet_ntoa(clientAddr->sin_addr);
    uint16_t port =  ntohs(clientAddr->sin_port);
    event_base* base = m_threadPool[currentThread].get_base();
    printf("线程:%d开始监听客户端%s\n",(int)m_threadPool[currentThread].get_threadId(),ip);
    currentThread = (currentThread+1)%m_threadNum;

    bufferevent* bev;
    bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    if(!bev)
    {
        printf("socket error");
        event_base_loopbreak(base);
        return;
    }

    bufferevent_setcb(bev,connect_readcb,NULL,event_callback,this);
    bufferevent_enable(bev,EV_WRITE);
    bufferevent_enable(bev,EV_READ);
    
    

}

void Tcpserver::connect_readcb(bufferevent* bev,void* user_data)
{
    Tcpserver* t = (Tcpserver*)user_data;
    t->readEvent(bev);  
    return ;
}

void Tcpserver::event_callback(bufferevent* bev,short events,void* user_data)
{
    printf("event_callback\n");
}

//虚函数定义
void Tcpserver::readEvent(bufferevent* bev)
{
    char buf[1024];
    bufferevent_read(bev,buf,1024);
    printf("%s\n",buf);
    char data[1024] = "nihao\n";
    bufferevent_write(bev,data,1024);
    
}

void Tcpserver::writeEvent()
{

}

void Tcpserver::closeEvent()
{

}
void Tcpserver::connectEvent()
{

}