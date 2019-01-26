cmake_minimum_required(VERSION 2.8)

project(graphy)

message(STATUS " building graphy ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)
include_directories(${TOP_SOURCE_PATH}/deps/lua/src)

file(GLOB SRC_FILES
    # sk views
    # ${SOURCE_UP_PATH}/skiall/src/views/*.* 
	# ${PROJECT_SOURCE_DIR}/view/*.*
		
    # self sources	
    ${PROJECT_SOURCE_DIR}/*.cpp 
    ${PROJECT_SOURCE_DIR}/*.h)

# add_definitions(-DGRAPHY_LIB)

set(VERSION 1.0.0)
add_library(graphy ${PROJECT_TYPE} ${SRC_FILES})
target_link_libraries(graphy skia resy)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "base") 
