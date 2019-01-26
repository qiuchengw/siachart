cmake_minimum_required(VERSION 2.8)

project(resy)

message(STATUS " building resy ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/zlib)
include_directories(${TOP_SOURCE_PATH}/deps/)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.*)
file(GLOB MINIZIP_FILES ${TOP_SOURCE_PATH}/deps/minizip/*.*)

# 我们不需要压缩的代码
list(REMOVE_ITEM MINIZIP_FILES ${TOP_SOURCE_PATH}/deps/minizip/minizip.c)
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "Windows: use minizip iowin32.")
ELSE ()
    # 不需要minizip的iowin32.h
    MESSAGE(STATUS "NOT Windows: remove minizip iowin32.")
    list(REMOVE_ITEM MINIZIP_FILES 
        ${TOP_SOURCE_PATH}/deps/minizip/iowin32.c
        ${TOP_SOURCE_PATH}/deps/minizip/iowin32.h)
ENDIF()

# add_definitions(-DRESY_LIB)

set(VERSION 1.0.0)

add_library(resy ${PROJECT_TYPE} ${SRC_FILES} ${MINIZIP_FILES})
target_link_libraries(resy zlib skia lua)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "base")
