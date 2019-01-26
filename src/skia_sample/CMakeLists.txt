cmake_minimum_required(VERSION 2.8)

project(skia_sample)

message(STATUS " building skia_sample ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

include(${TOP_SOURCE_PATH}/common.cmake)

file(GLOB SRC_FILES  
    ${SOURCE_UP_PATH}/skiall/samplecode/SampleAnimatedText.cpp
    ${SOURCE_UP_PATH}/skiall/samplecode/SampleCode.cpp
    ${SOURCE_UP_PATH}/skiall/samplecode/SampleCode.h
    ${SOURCE_UP_PATH}/skiall/samplecode/SampleChart.cpp)
# list(REMOVE_ITEM SRC_FILES 
#     ${SOURCE_UP_PATH}/skiall/samplecode/SampleBigGradient.cpp
#     ${SOURCE_UP_PATH}/skiall/samplecode/SampleBitmapRect.cpp
#     ${SOURCE_UP_PATH}/skiall/samplecode/SampleCamera.cpp
#     ${SOURCE_UP_PATH}/skiall/samplecode/SampleAndroidShadows.cpp)

set(VERSION 1.0.0)
add_executable(skia_sample ${SRC_FILES})
target_link_libraries(skia_sample skia graphy platy Opengl32)

