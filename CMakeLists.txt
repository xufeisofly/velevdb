# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.9)

# 项目信息
project (velevdb VERSION 1.22.0 LANGUAGES C CXX)

# C standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_C_STANDARD)
  # This project can use C11, but will gracefully decay down to C89.
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_C_STANDARD_REQUIRED OFF)
  set(CMAKE_C_EXTENSIONS OFF)
endif(NOT CMAKE_C_STANDARD)

# C++ standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_CXX_STANDARD)
  # This project requires C++11.
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  # set(CMAKE_CXX_COMPILER      "gcc" )         # 显示指定使用的C++编译器

  # set(CMAKE_CXX_FLAGS   "-std=c++11")             # c++11
  # set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
  # set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告

  # set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )             # 调试包不优化
  # set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )   # release包优化
endif(NOT CMAKE_CXX_STANDARD)

SET(BASEPATH "/Users/norris/projects/c/velevdb")
INCLUDE_DIRECTORIES("${BASEPATH}")


# 收集目录下所有的源文件，用于后面编译
aux_source_directory(db DIR_SRCS)

add_executable(debug "db/debug.cc" "db/coding.cc" "db/dbformat.cc" "db/memtable.cc")


# gtest 测试
# 查找 GTest 库
find_package(GTest REQUIRED)
# GTest 的头文件
include_directories(${GTEST_INCLUDE_DIRS})

list(REMOVE_ITEM DIR_SRCS "db/debug.cc")
add_executable(testall ${DIR_SRCS})

target_link_libraries(testall
  ${GTEST_BOTH_LIBRARIES}
  pthread)

gtest_discover_tests(testall)

enable_testing()
