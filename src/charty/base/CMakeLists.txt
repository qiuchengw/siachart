cmake_minimum_required(VERSION 2.8)

project(base)

message(STATUS " building base ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/lua/src)

file(GLOB SRC_FILES
    # self sources
    ${PROJECT_SOURCE_DIR}/*.*)

# add_definitions(-DBASE_LIB)

set(VERSION 1.0.0)
add_library(base ${PROJECT_TYPE} ${SRC_FILES})
target_link_libraries(base skia)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "chart")
