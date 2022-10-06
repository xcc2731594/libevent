#include "database.h"

Database::Database(const char* host,const char*userName,const char* password,const char* DbName)
{
    m_sql = mysql_init(NULL);
    if(NULL==m_sql)
    {
        printf("mysql error1");
        exit(1);
    }
    MYSQL* con = mysql_real_connect(m_sql,host,userName,password,DbName,0,NULL,0);
    if(NULL==con)
    {
        printf("con error2");
        exit(1);
    }
    m_sql = con;
    int ret = mysql_query(m_sql,"set names utf8;");
    if(0!=ret)
    {
        printf("query error3");
        exit(1);
    }
     
}

Database::~Database()
{}

bool Database::database_exec(const char*sql)
{
    std::unique_lock<std::mutex> loc(m_mutex);
    if(NULL==sql)
    {
        printf("error4");
        return false;
    }
    int ret = mysql_query(m_sql,sql);
    if(0!=ret)
    {
        printf("error5");
        return false;
    }
    return true;
}
bool Database::database_select(const char*sql,Json::Value &outjson)
{
    std::unique_lock<std::mutex> loc(m_mutex);
    if(NULL==sql)
    {
        printf("error6");
        return false;
    }
    int ret = mysql_query(m_sql,sql);
    if(0!=ret)
    {
        printf("error7");
        return false;
    }
    MYSQL_RES* res = mysql_store_result(m_sql);
    //查询结果为空
    if(NULL==res)
    {
        if(0==mysql_errno(m_sql))
        {
            return true;
        }
        else
        {
            printf("db_error");
            return false;
        }
    }
    //查询结果不为空
    //row:一行数据
    MYSQL_ROW row;
    //字段个数
    unsigned int num_field =  mysql_num_fields(res);
    //字段名
    MYSQL_FIELD * fetch_field = mysql_fetch_field(res);
    while(row = mysql_fetch_row(res))
    {
        for(int i=0;i<num_field;i++)
        {
            outjson[fetch_field[i].name].append(row[i]);
        }
    }
    //释放res
    mysql_free_result(res);


    return true;

}