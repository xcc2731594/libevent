#include "myserver.h"
#include <string>
Mserver::Mserver()
{
    //db
    m_db = new Database(NULL,"root","root","test");
    Json::Value outjson;
    char sql[] = "select * from user";
    m_db->database_select(sql,outjson);
    printf("%s\n",outjson["name"][0].toStyledString().c_str());
    std::string s = outjson.toStyledString();
    printf("----%s\n",s.c_str());
}

Mserver::~Mserver()
{

}

void Mserver::readEvent(bufferevent* bev)
{
    char buf[1024];
    bufferevent_read(bev,buf,1024);
    printf("%s\n",buf);
    char data[1024] = "nihao\n";
    bufferevent_write(bev,data,1024);
}
void Mserver::writeEvent()
{}
void Mserver::closeEvent()
{}
void Mserver::connectEvent()
{}

