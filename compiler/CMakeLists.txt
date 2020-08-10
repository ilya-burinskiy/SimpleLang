cmake_minimum_required(VERSION 3.5) 
project(scc)

set(CMAKE_BUILD_TYPE Debug)
set(SCC_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SCC_INC_DIR ${PROJECT_SOURCE_DIR}/inc)
set(SCC_SRCS ${SCC_SRC_DIR}/main.cpp 
             ${SCC_SRC_DIR}/lexer.cpp
             ${SCC_SRC_DIR}/word.cpp)
             

include_directories("${SCC_INC_DIR}")
add_executable(scc ${SCC_SRCS})