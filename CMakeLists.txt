cmake_minimum_required(VERSION 3.8)
project(scc)

set(CMAKE_BUILD_TYPE Debug)
set(SCC_INC_DIR ${PROJECT_SOURCE_DIR}/compiler/inc)
set(SCC_SRC_DIR ${PROJECT_SOURCE_DIR}/compiler/src)
set(SCC_SRCS ${SCC_SRC_DIR}/main.cpp 
             ${SCC_SRC_DIR}/token.cpp
             ${SCC_SRC_DIR}/lexer.cpp
             ${SCC_SRC_DIR}/ll1parser.cpp
             ${SCC_SRC_DIR}/ast.cpp
             ${SCC_SRC_DIR}/compiler.cpp
             ${SCC_SRC_DIR}/symbols.cpp)


include_directories("${SCC_INC_DIR}")
add_executable(scc ${SCC_SRCS})