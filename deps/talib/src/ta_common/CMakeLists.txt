cmake_minimum_required(VERSION 2.8)

project(ta_common)

message(STATUS " building ta_common ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include(${PROJECT_SOURCE_DIR}/../common.cmake)
# include_directories(${TOP_SOURCE_PATH}/deps/lua/src)

file(GLOB SRC_FILES
    # self sources
    ${PROJECT_SOURCE_DIR}/../../include/*.*
    ${PROJECT_SOURCE_DIR}/*.*)

set(VERSION 1.0.0)
add_library(ta_common ${SRC_FILES})
target_link_libraries(ta_common)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "talib")
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)