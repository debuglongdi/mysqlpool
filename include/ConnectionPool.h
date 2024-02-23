#pragma once
#include "noncopyable.h"
// #include "Connection.h"
#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace mysqlpool
{
/**
 * 实现连接池功能
*/
class Connection;
class ConnectionPool : noncopyable
{
public:
    //获取链接池对象实例
    static ConnectionPool* getConnectionPool();
    ~ConnectionPool();
    //从连接池队列中获取一个连接
    std::shared_ptr<Connection> getConnection();
    bool isExit() const { return isExit_; }
private:
    //构造函数私有
    ConnectionPool();
    
    //加载配置项
    void loadConfigFile();

    //生产连接的线程
    void produceConnectionTask();

    //清理空闲连接的线程
    void scannerConnectionTask();

    void setExit(){ isExit_ = true; }
    //一条sql连接用到的参数
    std::string ip_;
    unsigned short port_;
    std::string username_;
    std::string password_;
    std::string dbname_;

    int initSize_;//连接池的初始连接数
    int maxSize_;//最大连接数
    int maxIdleTime_;//连接池的最大空闲时间
    int connectionTimeout_;//连接池获取链技的超时时间

    //维护连接队列线程安全的互斥锁
    std::mutex mutex_;
    std::condition_variable cv_;
    //记录创建的链接数量
    std::atomic_int connectionCount_;
    //通知子线程关闭
    bool isExit_;

    // fix with std::queue< std::unique_ptr<Connection> > connectionQue_;
    std::queue<Connection*> connectionQue_;//存放所有连接的队列

    
};
} // namespace mysqlpool
