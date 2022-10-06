#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <mutex>
#include <jsoncpp/json/json.h>

class Database
{
private:
    MYSQL *m_sql;
    std::mutex m_mutex;

public:
    Database(const char* host,const char* userName,const char* password,const char* DnName);
    ~Database();

    bool database_exec(const char*sql);
    bool database_select(const char*sql,Json::Value &outjson);
};



#endif