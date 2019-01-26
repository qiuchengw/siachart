cmake_minimum_required(VERSION 2.8)

project(rastery)

message(STATUS " building rastery ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/lua/src)

file(GLOB SRC_FILES  ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/private/*.*)

# add_definitions(-DRASTERY_LIB)

set(VERSION 1.0.0)
add_library(rastery ${PROJECT_TYPE} ${SRC_FILES} ${SKAPP_FILES})
target_link_libraries(rastery  graphy)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "base")
