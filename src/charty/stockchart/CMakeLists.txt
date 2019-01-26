cmake_minimum_required(VERSION 2.8)

project(stockchart)

message(STATUS " building stockchart ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/lua/src)

file(GLOB SRC_FILES
    # self sources
    ${PROJECT_SOURCE_DIR}/*.cpp 
    ${PROJECT_SOURCE_DIR}/*.h)

# add_definitions(-DSTOCKCHART_LIB)

set(VERSION 1.0.0)
add_library(stockchart  ${SRC_FILES})
target_link_libraries(stockchart base charty skia resy graphy)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "chart")
