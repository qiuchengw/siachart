cmake_minimum_required(VERSION 2.8)

project(charty)

message(STATUS " building charty ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/lua/src)

file(GLOB SRC_FILES
    # self sources
    # ${PROJECT_SOURCE_DIR}/../base/*.*
    ${SKIA_PATH}/src/utils/SkTextUtils.cpp
    ${PROJECT_SOURCE_DIR}/*.cpp 
    ${PROJECT_SOURCE_DIR}/*.h)

# add_definitions(-DCHARTY_LIB)

set(VERSION 1.0.0)
add_library(charty ${PROJECT_TYPE} ${SRC_FILES})
target_link_libraries(charty base skia formula resy graphy)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "chart")
