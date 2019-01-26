cmake_minimum_required(VERSION 2.8)

project(gly)

message(STATUS " building gly ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)

file(GLOB SRC_FILES  ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/private/*.*)

# sk_app 公用的
file(GLOB SKAPP_FILES  
    # sk_app
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/Window.cpp 
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/Window.h 
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/WindowContext.h 
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/CommandSet.h 
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/CommandSet.cpp 
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/Application.h 
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/DisplayParams.h 
    # gl
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/GLWindowContext.cpp 
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/GLWindowContext.h 
    # raster
    ${SOURCE_UP_PATH}/skiall/tools/sk_app/RasterWindowContext.h)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    file(GLOB SKAPP_PLAT_FILES  
        ${SOURCE_UP_PATH}/skiall/src/utils/win/SkWGL_win.cpp
        ${SOURCE_UP_PATH}/skiall/tools/sk_app/win/GLWindowContext_win.h 
        ${SOURCE_UP_PATH}/skiall/tools/sk_app/win/GLWindowContext_win.cpp 
        ${SOURCE_UP_PATH}/skiall/tools/sk_app/win/main_win.cpp
        ${SOURCE_UP_PATH}/skiall/tools/sk_app/win/Window_win.cpp
        ${SOURCE_UP_PATH}/skiall/tools/sk_app/win/Window_win.h
        ${SOURCE_UP_PATH}/skiall/tools/sk_app/win/RasterWindowContext_win.cpp)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

# list(REMOVE_ITEM SRC_FILES  ${PROJECT_SOURCE_DIR}/dog.cpp ${PROJECT_SOURCE_DIR}/dog.h)

set(VERSION 1.0.0)
add_library(gly ${PROJECT_TYPE} ${SRC_FILES} ${SKAPP_FILES} ${SKAPP_PLAT_FILES})
target_link_libraries(gly skia graphy)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "base")
