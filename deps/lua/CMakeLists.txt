cmake_minimum_required(VERSION 2.8)

project(lua)

message(STATUS " building lua ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

# include(${TOP_SOURCE_PATH}/common.cmake)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.*)

# build as DLL
add_definitions(-DLUA_BUILD_AS_DLL)
# add_definitions(-DLUA_CORE)

set(VERSION 1.0.0)
add_library(lua ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "3rd")