#include "server.h"

// ChatDataBase *Server::chatdb = new ChatDataBase;
Server::Server(const char* ip,int port)
{
    chatlist = new ChatInfo;
    chatdb = new ChatDataBase;
    //创建事件集合
    base = event_base_new();
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    //创建监听对象
    listener = evconnlistener_new_bind(base,listener_cb,this,
    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,10,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(NULL==listen)
    {
        printf("evconnlistener_new_bind error\n");
    }
    cout<<"服务器初始化成功,开始监听客户端"<<endl;
    event_base_dispatch(base);
    // event_base_free(base);
}

void Server::listener_cb(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr *addr,int socklen,void *arg)
{
    printf("接收客户端的连接,fd=%d\n",fd);
    std::cout<<fd<<std::endl;
    //创建工作线程来处理该客户端
    std::thread client_thread(client_handler,fd,arg);
    client_thread.detach(); //线程分离

}

void Server::client_handler(int fd,void* arg)
{
    //创建集合
    struct event_base *base = event_base_new();
    //创建bufferevent对象
    struct bufferevent *bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    if(NULL==bev)
    {
      printf("bufferevent_socket_new error\n");  
    }
    //给bufferevent设置回调函数
    bufferevent_setcb(bev,read_cb,NULL,event_cb,(void*)arg);
    //使能回调函数
    bufferevent_enable(bev,EV_READ);

    event_base_dispatch(base);
    event_base_free(base);
}



void Server::read_cb(struct bufferevent *bev,void *ctx)
{
    Server * t =(Server*) ctx;
    char buf[1024] = {0};
    int size = bufferevent_read(bev,buf,sizeof(buf));
    if(size<0)
    {
        printf("bufferevent_read error\n");
    }
    std::cout<<buf<<std::endl;

    Json::Reader reader;    //解析json对象
    Json::FastWriter writer;  //解析json对象
    Json::Value val;
    if(!reader.parse(buf,val))
    {
         cout<<"服务器解析数据失败"<<endl;
    }
    string cmd = val["cmd"].asString();
    if(cmd == "register")  //注册功能
    {
         t->server_register(bev,val);
    }


}
void Server::event_cb(struct bufferevent *bev,short what,void *ctx)
{
    
}

Server::~Server()
{
    event_base_free(base);
}

void Server::server_register(struct bufferevent *bev,Json::Value val)
{
    chatdb->my_database_connect("user");
    if(chatdb->my_database_user_exist(val["user"].asString()))
    {
         Json::Value val;
         val["cmd"] = "register_reply";
         val["result"] = "failure";
         string b = Json::FastWriter().write(val);
         if(bufferevent_write(bev,b.c_str(),strlen(b.c_str()))<0)
         {
             cout<<"bufferevent_write"<<endl;
         }
    }
    else
    {
        chatdb->my_database_user_password(val["user"].asString(),val["password"].asString());
         Json::Value val;
         val["cmd"] = "register_reply";
         val["result"] = "success";
         string b = Json::FastWriter().write(val);
         if(bufferevent_write(bev,b.c_str(),strlen(b.c_str()))<0)
         {
             cout<<"bufferevent_write"<<endl;
         }
    }

    chatdb->my_database_disconnect();
}

