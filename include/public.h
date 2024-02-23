#pragma once
#include <iostream>
#include <string>
namespace mysqlpool
{
#define LOG(str) \
    std::cout<<__FILE__<<":"<<__LINE__<<" "<< \
    __TIMESTAMP__<<" : "<<str<<std::endl; \

} // namespace mysqlpool
