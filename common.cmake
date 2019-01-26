# skia

# IF(MSVC)
#     ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/xx")
# ELSE()
#     ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}")
# ENDIF()

# static / dll 
# [STATIC | SHARED | MODULE]
SET(PROJECT_TYPE SHARED)

IF (PROJECT_TYPE MATCHES "STATIC")
    add_definitions(-DSTATIC_LIB)
    MESSAGE(STATUS "build static project... ")
ELSEIF (PROJECT_TYPE MATCHES "SHARED")
    MESSAGE(STATUS "build dll project... ")
ENDIF()

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	MESSAGE(STATUS "current platform: FreeBSD")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Android")
    include_directories("${SKIA_PATH}/include/android")
ELSE ()
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF()

include_directories("${SOURCE_UP_PATH}")

# skia 
include_directories("${SKIA_PATH}")
include_directories("${SKIA_PATH}/include")
include_directories("${SKIA_PATH}/include/c")
include_directories("${SKIA_PATH}/include/config")
include_directories("${SKIA_PATH}/include/effects")
include_directories("${SKIA_PATH}/include/gpu")
include_directories("${SKIA_PATH}/include/ports")
include_directories("${SKIA_PATH}/include/svg")
include_directories("${SKIA_PATH}/include/views")
include_directories("${SKIA_PATH}/include/atlastext")
include_directories("${SKIA_PATH}/include/codec")
include_directories("${SKIA_PATH}/include/core")
include_directories("${SKIA_PATH}/include/encode")
include_directories("${SKIA_PATH}/include/pathops")
include_directories("${SKIA_PATH}/include/private")
include_directories("${SKIA_PATH}/include/utils")
# include_directories("${SKIA_PATH}/third_party/externals/sdl/include")
include_directories("${SKIA_PATH}/src/gpu")
include_directories("${SKIA_PATH}/src/core")
include_directories("${SKIA_PATH}/src/image")

include_directories("${SKIA_PATH}/tools/timer")
include_directories("${SKIA_PATH}/tools")
include_directories("${SKIA_PATH}/src/utils/")

# lua 使用skia中带的
include_directories("${SKIA_PATH}/third_party/externals/lua")
include_directories("${PROJECT_SOURCE_DIR}/deps/lua")

# sia项目自己的
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")

# deps的
include_directories("${PROJECT_SOURCE_DIR}/deps/zlib")


link_directories("${LIBRARY_OUTPUT_PATH}")
link_directories("${SKIA_PATH}/out/Shared")
# link_directories("${SKIA_PATH}/out/Debug")

add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DNOMINMAX)
add_definitions(-DDEBUG)

IF(MSVC)
    message(STATUS "--> using MSVC compiling........")
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    # ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/xx")
ELSE()
    # ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}")
ENDIF()
