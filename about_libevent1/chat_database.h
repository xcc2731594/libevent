#ifndef CHAT_DATABASE_H
#define CHAT_DATABASE_H
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <string.h>
using namespace std;
class ChatDataBase
{
private:
    MYSQL *mysql;
public:
    ChatDataBase();
    ~ChatDataBase();
    void my_database_connect(const char *);
    int my_database_get_group_name(string*);
    void my_database_get_group_member(string,string &);
    void my_database_disconnect();
    void my_database_user_password(string,string);
    bool my_database_user_exist(string);
};


#endif