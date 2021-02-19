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
endif(NOT CMAKE_CXX_STANDARD)

SET(BASEPATH "/Users/norris/projects/c/velevdb")
INCLUDE_DIRECTORIES("${BASEPATH}")

# include_directories(/Users/norris/projects/c/util)
# link_directories(/Users/norris/projects/leveldb/build)

add_executable(velevdb "db/velevdbutil.cc")


# 查找 GTest 库
find_package(GTest REQUIRED)
# GTest 的头文件
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(skiplisttest "db/skiplist_test.cc")

target_link_libraries(skiplisttest
  ${GTEST_BOTH_LIBRARIES}
  pthread)

gtest_discover_tests(skiplisttest)

enable_testing()

# 指定生成目标

# target_link_libraries(Demo leveldb)
# target_link_libraries(Demo snappy)
