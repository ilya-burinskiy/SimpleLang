cmake_minimum_required(VERSION 3.5) 
project(svm)


set(SVM_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SVM_INC_DIR ${PROJECT_SOURCE_DIR}/inc)
set(SVM_SRCS ${SVM_SRC_DIR}/main.cpp ${SVM_SRC_DIR}/svm.cpp)

include_directories("${SVM_INC_DIR}")
add_executable(svm ${SVM_SRCS})