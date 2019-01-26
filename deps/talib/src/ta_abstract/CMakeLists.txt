cmake_minimum_required(VERSION 2.8)

project(ta_abstract)

message(STATUS " building ta_abstract ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include(${PROJECT_SOURCE_DIR}/../common.cmake)

file(GLOB SRC_FILES
    # self sources
    ${PROJECT_SOURCE_DIR}/../../include/*.*
    ${PROJECT_SOURCE_DIR}/frames/*.*
    ${PROJECT_SOURCE_DIR}/tables/*.*
    ${PROJECT_SOURCE_DIR}/templates/*.*
    ${PROJECT_SOURCE_DIR}/*.*)

# file(REMOVE ${PROJECT_SOURCE_DIR}/excel_glue.c) 
list(REMOVE_ITEM SRC_FILES "${PROJECT_SOURCE_DIR}/excel_glue.c")

set(VERSION 1.0.0)
add_library(ta_abstract ${SRC_FILES} ${REMOVE_ITEM})
target_link_libraries(ta_abstract ta_common ta_func)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES FOLDER "talib")
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)