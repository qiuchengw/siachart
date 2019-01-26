cmake_minimum_required(VERSION 2.8)

project(zlib)

add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DZLIB_DLL)
add_definitions(-DZLIB_INTERNAL)
add_definitions(-DNO_vsnprintf)

message(status " build zlib ... ")
message(status " source dir: " ${zlib_SOURCE_DIR})

file(GLOB SRC_FILES ${zlib_SOURCE_DIR}/*.c ${zlib_SOURCE_DIR}/*.h)

list(REMOVE_ITEM SRC_FILES  ${zlib_SOURCE_DIR}/zlib-dllversion.c)


set(VERSION 1.0.0)

add_library(zlib SHARED ${SRC_FILES})
SET_TARGET_PROPERTIES(zlib PROPERTIES LINKER_LANGUAGE CXX SOVERSION ${VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "3rd")