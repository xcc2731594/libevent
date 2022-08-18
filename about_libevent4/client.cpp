#include <event2/event.h>
#include <event2/bufferevent.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
void eventcb(struct bufferevent* bev,short events,void *ptr)
{
    if(events&BEV_EVENT_CONNECTED)
    {
        std::cout<<"connection established;"<<std::endl;
    }
    else if(events & BEV_EVENT_ERROR)
    {

    }
}

void read_terminal_cb(evutil_socket_t fd,short what,void *arg)
{
    char buf[BUFSIZ] = {0};
    int len= read(fd,buf,sizeof(buf));
    struct bufferevent *bev = (struct bufferevent*)arg;
    bufferevent_write(bev,buf,len);

}

int main()
{
    struct event_base *base;
    struct bufferevent *bev;
    struct sockaddr_in sin;
    
    base = event_base_new();
    memset(&sin,0,sizeof(sin));
    sin.sin_family=AF_INET;
    // sin.sin_addr.s_addr=htonl(0x7f000001);
    sin.sin_port=htons(8989);
    bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev,NULL,NULL,eventcb,NULL);
    if(bufferevent_socket_connect(bev,
    (struct sockaddr*)&sin,sizeof(sin))<0)
    {
        bufferevent_free(bev);
        return -1;

    }
    struct event *ev = event_new(base,0,EV_READ|EV_PERSIST,read_terminal_cb,bev);
    event_add(ev,NULL);
    event_base_dispatch(base);
    event_free(ev);
    bufferevent_free(bev);
    event_base_free(base);
    return 0;
}
