cmake_minimum_required(VERSION 2.8)

project(demo_graphy_gl)

message(STATUS " building demo_graphy_gl ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)

file(GLOB SRC_FILES  ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB COMMON_FILES  ${PROJECT_SOURCE_DIR}/../mgraphy/*.*)

set(VERSION 1.0.0)
add_executable(demo_graphy_gl ${SRC_FILES} ${COMMON_FILES})
target_link_libraries(demo_graphy_gl skia graphy gly Opengl32)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "demo")
