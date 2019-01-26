cmake_minimum_required(VERSION 2.8)

project(demo_lua)

message(STATUS " building demo_lua ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)

file(GLOB SRC_FILES  ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)

set(VERSION 1.0.0)
add_executable(demo_lua ${SRC_FILES})
target_link_libraries(demo_lua skia)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "demo")
