cmake_minimum_required(VERSION 2.8)

project(demo_resy)

message(STATUS " building demo_resy ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/zlib)

file(GLOB SRC_FILES  ${PROJECT_SOURCE_DIR}/*.*)

set(VERSION 1.0.0)
add_executable(demo_resy ${SRC_FILES})
target_link_libraries(demo_resy resy zlib skia lua)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "demo")
