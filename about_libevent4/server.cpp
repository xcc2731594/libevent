#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <string.h>
#include <iostream>

static void on_read(struct bufferevent *bev,void *arg)
{
    struct evbuffer *evbuf = bufferevent_get_input(bev);
    // char *msg = evbuffer_readln(evbuf,NULL,EVBUFFER_EOL_CRLF);
    char buf[1024] = {0};
    bufferevent_read(bev,buf,sizeof(buf));
    // if(!msg)return;
    printf("server read the data:%s\n",buf);
}
static void on_error(struct bufferevent *bev,short events,void *arg)
{
    if(events&BEV_EVENT_EOF)
        printf("connection closed\n");
    else if(events&BEV_EVENT_ERROR)
        printf("some other error\n");
    else if(events&BEV_EVENT_TIMEOUT)
        printf("timeout\n");
    bufferevent_free(bev);
}

static void on_open(struct evconnlistener *listener,
                    evutil_socket_t fd,
                    struct sockaddr *sock,
                    int socklen,
                    void *arg)
                    {
    char ip[32] = {0};
    evutil_inet_ntop(AF_INET,sock,ip,sizeof(ip)-1);
    printf("accept a client fd:%d ip:%s\n",fd,ip);
    struct event_base *base = (struct event_base*)arg;
    struct bufferevent *bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev,on_read,NULL,on_error,NULL);
    bufferevent_enable(bev,EV_READ|EV_PERSIST);
}
int main()
{
    struct event_base *base = event_base_new();
    struct sockaddr_in sin;
    memset(&sin,0,sizeof(struct sockaddr_in));
    sin.sin_family =AF_INET;
    sin.sin_port = htons(8989);
    struct evconnlistener *listener = 
        evconnlistener_new_bind(base,on_open,base,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
        512,(struct sockaddr*)&sin,sizeof(struct sockaddr_in));
    if(!listener)
    {
        std::cout<<"could not create a listener"<<std::endl;
        return 1;
    }
    printf("listening\n");    
    event_base_dispatch(base);
    evconnlistener_free(listener);
    event_base_free(base);
    printf("end!\n");

}