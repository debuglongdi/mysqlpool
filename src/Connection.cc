#include "Connection.h"
#include "public.h"

using namespace mysqlpool;

Connection::Connection()
{
    _conn = mysql_init(nullptr);
}

Connection::~Connection()
{
    if(_conn != nullptr)
    {
        mysql_close(_conn);
    }
}

bool Connection::connect(std::string ip,
    unsigned short port,
    std::string username,
    std::string password,
    std::string dbname)
{
    MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(), password.c_str(), dbname.c_str(),port,nullptr,0);
    return p != nullptr;
}

bool Connection::update(std::string sql)
{
    //更新 0 on success other fault 
    if(mysql_query(_conn, sql.c_str()))
    {
        LOG("update failuer %s\n");
        return false;
    }
    return true;
}


MYSQL_RES* Connection::query(std::string sql)
{
    //查询 select
    if(mysql_query(_conn, sql.c_str()))
    {
        LOG("select failuer %s\n");
        return nullptr;
    }
    return mysql_use_result(_conn);
}