cmake_minimum_required(VERSION 4.2)
project(UDS_Service_BBB)

include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/transport) # Include all subfolders recursively
include_directories(${CMAKE_SOURCE_DIR}/include/appl) # Include all subfolders recursively
#GLOB_RECURSE: tranverse all the file inside src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(UDS_Service_BBB main.cpp ${SOURCES})

