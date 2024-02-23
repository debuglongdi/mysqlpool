#pragma once
#include <mysql/mysql.h>
#include <string>
namespace mysqlpool
{
/**
 * 实现MySQL数据库的增删改查
 * */ 
class Connection
{
public:
    /// @brief 初始化数据库连接
    Connection();
    /// @brief 释放数据库连接
    ~Connection();
    
    /// @brief 建立数据库连接
    /// @param ip ip地址
    /// @param port 端口号
    /// @param user 用户名
    /// @param password 密码
    /// @param dbname 数据库名
    /// @return 
    bool connect(std::string ip,
        unsigned short port,
        std::string user,
        std::string password,
        std::string dbname);
    
    /// @brief 执行更新操作 insert delete update
    /// @param sql add/del/mod 的sql语句
    /// @return 
    bool update(std::string sql);

    /// @brief 执行查询操作
    /// @param sql 查询的sql语句
    /// @return 查询的结果，类型:MYSQL_RES*
    MYSQL_RES* query(std::string sql);

    void refreshAliveTime()
    { aliveTime_ = clock(); }
    clock_t getAliveTime() const 
    { return clock() - aliveTime_; }
    
private:
    MYSQL *_conn;
    clock_t aliveTime_;
};
} // namespace mysqlpool
