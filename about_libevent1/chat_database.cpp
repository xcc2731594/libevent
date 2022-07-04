#include "chat_database.h"
using namespace std;
ChatDataBase::ChatDataBase()
{
    

}

ChatDataBase::~ChatDataBase()
{
    mysql_close(mysql);
}

void ChatDataBase::my_database_connect(const char *name)
{
    
    mysql = mysql_init(NULL); 
    mysql = mysql_real_connect(mysql,"localhost","root","root",name,0,NULL,0);
    if(NULL==mysql)
    {
        cout<<"connect database failure"<<endl;
    }
}

int ChatDataBase::my_database_get_group_name(string *s)
{
    if(mysql_query(mysql,"set names utf8;")!=0)
    {
        cout<<"mysql_query error"<<endl;
    }
    if(mysql_query(mysql,"show tables;")!=0)
    {
        cout<<"mysql_query error"<<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    if(NULL==res)
    {
        cout<<"mysql_store_result"<<endl;
    }
    MYSQL_ROW row;
    int count=0;
    while(row = mysql_fetch_row(res))
    {
        s[count]+= row[0];
        count++;
    }
    return count;

}


void ChatDataBase::my_database_get_group_member(string name,string &s)
{
    char sql[1024] = {0};
    sprintf(sql,"select member from %s;",name.c_str());
    if(mysql_query(mysql,sql)!=0)
    {
        cout<<"mysql_query error"<<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    if(NULL==res)
    {
        cout<<"mysql_store_result error"<<endl;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    s += row[0];

}

void ChatDataBase::my_database_disconnect()
{
    mysql_close(mysql);
}

bool ChatDataBase::my_database_user_exist(string name)
{
    if(mysql_query(mysql,"set names utf8;")!=0)
    {
        cout<<"mysql_query error"<<endl;
    }
    char sql[128]={0};
    sprintf(sql,"show tables like '%s';",name.c_str());
    if (mysql_query(mysql,sql)!=0)
    {
        cout<<"mysql_query error"<<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    if(NULL==row)
    {
        return false;
    }
    else
    {
        return true;
    }
}


void ChatDataBase::my_database_user_password(string name,string password)
{
    char sql[128] = {0};
    sprintf(sql,"create table %s(password varchar(16),friend varchar(4096));",
    name.c_str());
    if(mysql_query(mysql,sql)!=0)
    {
        cout<<"mysql_query error"<<endl;
    }
    memset(sql,0,sizeof(sql));
    sprintf(sql,"insert into %s(password) values ('%s');",
    name.c_str(),password.c_str());
    if(mysql_query(mysql,sql)!=0)
    {
        cout<<"mysql_query error"<<endl;
    }

}
