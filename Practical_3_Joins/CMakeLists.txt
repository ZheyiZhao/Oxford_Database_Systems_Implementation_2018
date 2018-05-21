cmake_minimum_required (VERSION 2.4)
project (minibase)

set(CMAKE_CXX_FLGAS "-Wall -O0")
set(CMAKE_BUILD_TYPE Debug)

find_library(BUFMGR_LIB bufmgr lib/)
find_library(SPACEMGR_LIB spacemgr lib/)
find_library(BTREE_LIB btree lib/)
find_library(GLOBALDEFS_LIB globaldefs lib/)
find_library(JOINS_LIB joins lib/)

add_subdirectory(joins)

add_executable (minibase-joins main.cpp)
target_link_libraries (minibase-joins joins ${BTREE_LIB} ${SPACEMGR_LIB} ${BUFMGR_LIB} ${GLOBALDEFS_LIB} ${SPACEMGR_LIB} ${BUFMGR_LIB}) 
