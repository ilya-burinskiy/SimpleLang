cmake_minimum_required(VERSION 3.8)
project(lexer)

set(CMAKE_BUILD_TYPE Debug)
set(LEX_INC_DIR ${PROJECT_SOURCE_DIR}/inc)
set(LEX_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LEX_SRCS ${LEX_SRC_DIR}/lexer/main.cpp 
             ${LEX_SRC_DIR}/lexer/lexer.cpp
             ${LEX_SRC_DIR}/word.cpp
             ${LEX_SRC_DIR}/token.cpp
             ${LEX_SRC_DIR}/num.cpp
             ${LEX_SRC_DIR}/operation.cpp)

include_directories("${LEX_INC_DIR}")
add_executable(lex ${LEX_SRCS})