#include "ConnectionPool.h"
#include "Connection.h"
#include <chrono>
#include <thread>
#include <iostream>
using namespace std;
using namespace mysqlpool;

int main()
{
    //多线程
    const int NUM = 1000 ;
    cout<<"4 thread, data_number="<<NUM<<endl;
    char sql[1024];
    sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");

        //////开始
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1([](){
        int num = NUM/4;
        char sql[1024];
        sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");
        for(int i=0;i<num;i++)
        {
            ConnectionPool* pool = ConnectionPool::getConnectionPool();
            std::shared_ptr<Connection> conn = pool->getConnection();
            conn->update(sql);
        }
    });
    std::thread t2([](){
        int num = NUM/4;
        char sql[1024];
        sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");
        for(int i=0;i<num;i++)
        {
            ConnectionPool* pool = ConnectionPool::getConnectionPool();
            std::shared_ptr<Connection> conn = pool->getConnection();
            conn->update(sql);
        }
    });
    std::thread t3([](){
        int num = NUM/4;
        char sql[1024];
        sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");
        for(int i=0;i<num;i++)
        {
            ConnectionPool* pool = ConnectionPool::getConnectionPool();
            std::shared_ptr<Connection> conn = pool->getConnection();
            conn->update(sql);
        }
    });
    std::thread t4([](){
        int num = NUM/4;
        char sql[1024];
        sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");
        for(int i=0;i<num;i++)
        {
            ConnectionPool* pool = ConnectionPool::getConnectionPool();
            std::shared_ptr<Connection> conn = pool->getConnection();
            conn->update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();
    //////结束
    // 计算并输出耗时（单位：毫秒）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "该操作耗时: " << duration.count() << " 毫秒" << std::endl;
#if 0
    // 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();
    //初始化连接池
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    for(int i=0;i<NUM;i++)
    {
        //从连接池中获取一条连接
        std::shared_ptr<Connection> conn = pool->getConnection();
        
        //插入数据
        conn->update(sql);
        
        //离开作用域后，连接自动归还
    }
    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();

    // 计算并输出耗时（单位：毫秒）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "该操作耗时: " << duration.count() << " 毫秒" << std::endl;

#endif

    return 0;
}