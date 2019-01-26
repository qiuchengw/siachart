cmake_minimum_required(VERSION 2.8)

project(formula)

message(STATUS " building formula ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)

file(GLOB SRC_FILES
    # self sources
    # ${PROJECT_SOURCE_DIR}/../base/*.*
    ${PROJECT_SOURCE_DIR}/*.*)

# add_definitions(-Dformula_LIB)

set(VERSION 1.0.0)
add_library(formula ${PROJECT_TYPE} ${SRC_FILES})
target_link_libraries(formula base skia resy graphy ta_func ta_common ta_abstract)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "chart")
