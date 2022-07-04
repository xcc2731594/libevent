#ifndef CHATINFO_H
#define CHATINFO_H

#include <list>
#include <event.h>
#include <string>
#include "chat_database.h"
using namespace std;

struct User
{
    string name;
    struct bufferevent *bev;
};

struct GroupUser
{
    string name;
};

struct Group
{
    string name;
    list<GroupUser> *l;

};
class ChatInfo
{
private:
    list<User> *online_user;
    list<Group> *group_info;
    ChatDataBase *mydatabase;

public:
    ChatInfo();
    ~ChatInfo();
};



#endif