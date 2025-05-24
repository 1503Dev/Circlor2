cmake_minimum_required(VERSION 3.4.1)

# 设置项目名称
project(Circlor2)

# 设置编译选项
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# 添加共享库
add_library(
        circlor2
        SHARED
        circlor2.cpp
)

# 设置目标属性
set_target_properties(
        circlor2
        PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)

# 链接标准库
target_link_libraries(
        circlor2
        c++_static
)