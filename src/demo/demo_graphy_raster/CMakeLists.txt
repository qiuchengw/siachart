cmake_minimum_required(VERSION 2.8)

project(demo_graphy_raster)

message(STATUS " building demo_graphy_raster ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/lua/src)

file(GLOB SRC_FILES  ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB COMMON_FILES  ${PROJECT_SOURCE_DIR}/../mgraphy/*.*)

set(VERSION 1.0.0)
add_executable(demo_graphy_raster ${SRC_FILES} ${COMMON_FILES})
target_link_libraries(demo_graphy_raster skia graphy rastery lua resy)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "demo")
