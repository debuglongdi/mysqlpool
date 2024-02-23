#include <iostream>
#include "public.h"
#include "Connection.h"
#include "ConnectionPool.h"
using namespace std;

using namespace mysqlpool;
int main()
{
    /*
    Connection conn;
    char sql[1024];
    sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "zhang san", 20, "male");
    //建立连接
    bool flag = conn.connect("127.0.0.1", 3306, "sqlpool", "123456", "chat");
    if(flag == false){
        cout<<"连接失败\n";
    }
    conn.update(sql);
    */

    // ConnectionPool *pool = ConnectionPool::getConnectionPool();
    // pool->loadConfigFile();
    char sql[1024];
    sprintf(sql,"insert into user(name,age,sex) values('%s','%d','%s')",
            "zhang san", 99, "male");
    ConnectionPool *pool = ConnectionPool::getConnectionPool();
    std::shared_ptr<Connection> p = pool->getConnection();
    p->update(sql);
    
    return 0;
}
