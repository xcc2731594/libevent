#ifndef MYSERVER_H
#define MYSERVER_H

#include "tcpserver.h"
//db
#include "database.h"
class Mserver:public Tcpserver
{
public:
    Mserver();
    ~Mserver();

    void readEvent(bufferevent* bev);
    void writeEvent();
    void closeEvent();
    void connectEvent();
private:
//db
    Database *m_db;
};


#endif