cmake_minimum_required(VERSION 3.16)
project(mysqlpool CXX)

# 设置调试信息 以及启动c++11语言标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 设置Debug构建类型的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Wall -std=c++11")

# 设置Release构建类型的编译选项
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -std=c++11")

#设置构建时使用的版本 Release or Debug
set(CMAKE_BUILD_TYPE Release) # 或 Release

#如果想要DEBUG的输出，需要定义HONG宏
# add_compile_definitions(HONG)

# 添加包含文件，为了使得我们编写的 .cpp 源文件能够使用在不同目录里的包含文件
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#设置动态库输出文件
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



# 生成库文件mysqlpool，第一个参数是我们希望生成的库的名字，后续是生成这个库所需要的所有源文件
add_library(mysql_pool SHARED
        src/ConnectionPool.cc
        src/Connection.cc
        )

#########################################################################################
#########################################################################################

#生成第一个可执行文件,生成test可执行文件
add_executable(main src/main.cc)
target_link_libraries(main mysql_pool mysqlclient)

#生成test1可执行文件,生成test可执行文件
add_executable(test1 src/nonuse_sqlpool.cc)
target_link_libraries(test1 mysql_pool mysqlclient)

#生成test1可执行文件,生成test可执行文件
add_executable(test2 src/use_sqlpool.cc)
target_link_libraries(test2 mysql_pool mysqlclient)


#########################################################################################
#########################################################################################
