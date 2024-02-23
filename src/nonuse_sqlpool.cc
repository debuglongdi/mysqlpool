#include "Connection.h"
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
using namespace mysqlpool;
struct config
{
    std::string ip_;
    unsigned short port_;
    std::string username_;
    std::string password_;
    std::string dbname_;
    int initSize_;
    int maxSize_;
    int maxIdleTime_;
    int connectionTimeout_;
    /* data */
};
struct config cf;


void loadConfigFile()
{
    FILE *pf = fopen("../mysql.cnf","r");
    if(pf == nullptr)
    {
        printf("open config file fail\n");
        exit(0);
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
            cf.ip_ = value;
        }
        else if(key == "port")
        {
            cf.port_ = (unsigned short)stoi(value);
        }
        else if(key == "username")
        {
            cf.username_ = value;
        }
        else if(key == "password")
        {
            cf.password_ = value;
        }
        else if(key == "dbname")
        {
            cf.dbname_ = value;
        }
        else if(key == "initSize")
        {
            cf.initSize_ = stoi(value);
        }
        else if(key == "maxSize")
        {
            cf.maxSize_ = stoi(value);
        }
        else if(key == "maxIdleTime")
        {
            cf.maxIdleTime_ = stoi(value);
        }
        else if(key == "connectionTimeout")
        {
            cf.connectionTimeout_ = stoi(value);
        }
    }
    //cout<<ip_<<":"<<port_<<":"<<username_<<":"<<password_<<":"<<dbname_<<"\n";
    //printf("initSize=%d maxSize=%d maxIdleTime=%ds connectionTimeout=%dms\n",initSize_,maxSize_,maxIdleTime_,connectionTimeout_);
}



int main()
{
    //加载配置文件
    loadConfigFile();
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
            Connection conn;
            //连接数据库连接
            bool flag = conn.connect(cf.ip_,cf.port_,cf.username_, cf.password_, cf.dbname_);
            if(flag == false){
                cout<<"连接失败\n";
            }
            //插入数据
            conn.update(sql);
            
            //离开作用域后，连接断开
        }
    });
    std::thread t2([](){
        int num = NUM/4;
        char sql[1024];
        sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");
        for(int i=0;i<num;i++)
        {
            Connection conn;
            //连接数据库连接
            bool flag = conn.connect(cf.ip_,cf.port_,cf.username_, cf.password_, cf.dbname_);
            if(flag == false){
                cout<<"连接失败\n";
            }
            //插入数据
            conn.update(sql);
            
            //离开作用域后，连接断开
        }
    });
    std::thread t3([](){
        int num = NUM/4;
        char sql[1024];
        sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");
        for(int i=0;i<num;i++)
        {
            Connection conn;
            //连接数据库连接
            bool flag = conn.connect(cf.ip_,cf.port_,cf.username_, cf.password_, cf.dbname_);
            if(flag == false){
                cout<<"连接失败\n";
            }
            //插入数据
            conn.update(sql);
            
            //离开作用域后，连接断开
        }
    });
    std::thread t4([](){
        int num = NUM/4;
        char sql[1024];
        sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "Li bai", 99, "male");
        for(int i=0;i<num;i++)
        {
            Connection conn;
            //连接数据库连接
            bool flag = conn.connect(cf.ip_,cf.port_,cf.username_, cf.password_, cf.dbname_);
            if(flag == false){
                cout<<"连接失败\n";
            }
            //插入数据
            conn.update(sql);
            
            //离开作用域后，连接断开
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
    for(int i=0;i<NUM;i++)
    {
        Connection conn;
        //连接数据库连接
        bool flag = conn.connect(cf.ip_,cf.port_,cf.username_, cf.password_, cf.dbname_);
        if(flag == false){
            cout<<"连接失败\n";
        }
        //插入数据
        conn.update(sql);
        
        //离开作用域后，连接断开
    }
    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();

    // 计算并输出耗时（单位：毫秒）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "该操作耗时: " << duration.count() << " 毫秒" << std::endl;
#endif
    return 0;
}