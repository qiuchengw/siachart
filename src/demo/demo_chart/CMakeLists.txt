cmake_minimum_required(VERSION 2.8)

project(demo_chart)

message(STATUS " building demo_chart ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/zlib)

file(GLOB SRC_FILES  
    ${PROJECT_SOURCE_DIR}/../mgraphy/*.*
    ${PROJECT_SOURCE_DIR}/*.*)

set(VERSION 1.0.0)
add_executable(demo_chart ${SRC_FILES})
target_link_libraries(demo_chart resy zlib skia lua rastery charty stockchart formula)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "demo")
