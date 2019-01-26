cmake_minimum_required(VERSION 2.8)

project(demo_qt)

message(STATUS " building demo_qt ...")
message(STATUS " source dir: " ${PROJECT_SOURCE_DIR})

# qt
set(CMAKE_PREFIX_PATH "c:/Qt/Qt5.11.0/")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# find_package(Qt5Widgets REQUIRED)

# source
include(${TOP_SOURCE_PATH}/common.cmake)
file(GLOB SRC_FILES  ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)

set(VERSION 1.0.0)
add_executable(demo_qt ${SRC_FILES})
# target_link_libraries(demo_qt ${Qt5Widgets_LIBRARIES} skiall graphy rastery)
qt5_use_modules(demo_qt Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "demo")
