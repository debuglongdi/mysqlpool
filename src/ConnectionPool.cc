#include "ConnectionPool.h"
#include "Connection.h"
#include "public.h"
#include <thread>
#include <functional>

using namespace mysqlpool;


ConnectionPool::ConnectionPool()
    : mutex_(),
    cv_(),
    connectionCount_(0),
    isExit_(false)
{
    //加载配置项，失败直接退出了
    loadConfigFile();

    //创建初始连接
    for(int i = 0; i < initSize_; i++ )
    {
        Connection *p = new Connection();
        bool res = p->connect(ip_,port_,username_,password_,dbname_);
        if(!res)
        {
            continue;
            //是否要尝试重新连接是一个问题
            //因为不知道是mysql出错了
            //还是网络出错
            //还是mysql服务器处理的连接过多，当前太忙
        }
        //刷新一下开始空闲的时间
        p->refreshAliveTime();
        connectionQue_.push(p);
        connectionCount_++;
    }

    //启动一个线程作为生产者线程
    std::thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));
    produce.detach();

    //启动一个新的定时线程，扫描超过maxIdleTime的空闲连接，进行连接回收
    std::thread scanner(std::bind(&ConnectionPool::scannerConnectionTask,this));
    scanner.detach();
}

ConnectionPool::~ConnectionPool()
{
    //设置退出标志，并通知所有线程
    setExit();
    cv_.notify_all();
    //清理连接池
    std::unique_lock<std::mutex> lock(mutex_);
    while(!connectionQue_.empty())
    {
        auto p = connectionQue_.front();
        delete p;
        connectionQue_.pop();
    }
}



ConnectionPool* ConnectionPool::getConnectionPool()
{
    //静态局部变量初始化由系统完成，且只初始化一次，且它的生命周期是全局的
    static ConnectionPool pool;
    return &pool;
}

void ConnectionPool::loadConfigFile()
{
    //在上级目录找
    FILE *pf = fopen("../mysql.cnf","r");
    if(pf == nullptr)
    {
        //在同级目录找
        pf = fopen("./mysql.cnf","r");
        if(pf == nullptr)
        {
            LOG("can't open config file fail\n");
            exit(0);
        }
    }
    while(!feof(pf))
    {
        char line[1024] = {0};
        if(fgets(line, 1024, pf) == nullptr)
            continue;
        std::string str = line;
        int idx = str.find('=',0);
        if(idx ==-1) //无效配置项
        {
            continue;
        }
        int endix = str.find('\n',idx);
        std::string key = str.substr(0,idx);
        std::string value = str.substr(idx+1,endix-idx-1);
        //填入数据
        if(key == "ip")
        {
            ip_ = value;
        }
        else if(key == "port")
        {
            port_ = (unsigned short)stoi(value);
        }
        else if(key == "username")
        {
            username_ = value;
        }
        else if(key == "password")
        {
            password_ = value;
        }
        else if(key == "dbname")
        {
            dbname_ = value;
        }
        else if(key == "initSize")
        {
            initSize_ = stoi(value);
        }
        else if(key == "maxSize")
        {
            maxSize_ = stoi(value);
        }
        else if(key == "maxIdleTime")
        {
            maxIdleTime_ = stoi(value);
        }
        else if(key == "connectionTimeout")
        {
            connectionTimeout_ = stoi(value);
        }
    }
    //cout<<ip_<<":"<<port_<<":"<<username_<<":"<<password_<<":"<<dbname_<<"\n";
    //printf("initSize=%d maxSize=%d maxIdleTime=%ds connectionTimeout=%dms\n",initSize_,maxSize_,maxIdleTime_,connectionTimeout_);
}

//从连接池队列中获取一个连接
std::shared_ptr<Connection> ConnectionPool::getConnection()
{
    std::unique_lock<std::mutex> lock(mutex_);
    while(connectionQue_.empty())
    {
        if( std::cv_status::timeout == cv_.wait_for(lock, std::chrono::milliseconds(connectionTimeout_)) )
        {
            if(connectionQue_.empty())
            {
                LOG("get conn failed,please retry later\n");
                return nullptr;
            }
        }
    }

    //成功了
    std::shared_ptr<Connection> up(connectionQue_.front(),[&](Connection *pcon){
        //归还连接时，需要锁
        std::unique_lock<std::mutex> lock(mutex_);
        //刷新连接空闲的起始时间
        pcon->refreshAliveTime();
        connectionQue_.push(pcon);
    });
    connectionQue_.pop();

    //通知生产者，如果队列中连接为空，赶紧生产连接
    cv_.notify_all();
    return up;
}


//生产连接的线程
void ConnectionPool::produceConnectionTask()
{
    while(!isExit())
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while(!connectionQue_.empty())
        {
            //等待队列为空
            cv_.wait(lock);
            if(isExit()) break;
        }
        if(isExit()) break;
        //连接数量没有到达上限
        if(connectionCount_ < maxSize_)
        {
            Connection *p = new Connection();
            p->connect(ip_,port_,username_,password_,dbname_);
            p->refreshAliveTime();//刷新连接的空闲起始时间
            connectionQue_.push(p);
            connectionCount_++;
        }
        //通知销费者线程可以销费了
        cv_.notify_all();
    }
}

//清理空闲连接的线程
void ConnectionPool::scannerConnectionTask()
{
    while(!isExit())
    {
        //通过sleep模拟定时效果
        if(isExit()) break;
        std::this_thread::sleep_for(std::chrono::seconds(maxIdleTime_));
        if(isExit()) break;
        while(connectionCount_ > initSize_)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            Connection *p = connectionQue_.front();
            //超过最大空闲时间还是空闲的,释放连接
            if(p->getAliveTime() >= (maxIdleTime_ * 1000))
            {
                connectionQue_.pop();
                connectionCount_--;
                delete p;
            }
            else
            {
                break;//队头的连接没有超过最大空闲时间，其他的也一定没有超过
            }
        }
    }
}