cmake_minimum_required(VERSION 3.8)
set(PROJECT_NAME CSUGL)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)

# GLFW
set(GLFW_DIR vendor/GLFW)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)
LIST(APPEND LIBS glfw ${GLFW_LIBRARIES})

# glad
set(GLAD_DIR vendor/glad)
set(GLAD_SRC ${GLAD_DIR}/src/glad.c)
include_directories(${GLAD_DIR}/include)

# glm
set(GLM_DIR vendor/glm)
include_directories(${GLM_DIR})

# glog
add_subdirectory(vendor/glog)
list(APPEND LIBS glog::glog)
include_directories(vendor/glog/src)

# stb
add_subdirectory(vendor/stb)
list(APPEND LIBS STB)
include_directories(vendor/stb)

# imgui
add_subdirectory(vendor/imgui)
list(APPEND LIBS imgui)
include_directories(vendor/imgui)

option(CSUGL_BUILD_MODEL_LOADER OFF)

if(CSUGL_BUILD_MODEL_LOADER)
    # assimp
    message("Assimp is On")
    option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
    option(ASSIMP_BUILD_SAMPLES OFF)
    option(ASSIMP_BUILD_TESTS OFF)
    OPTION(ASSIMP_BUILD_ZLIB "" ON)
    add_subdirectory(vendor/assimp)
    list(APPEND LIBS assimp)
    include_directories(vendor/assimp/include)
endif()

# CSUGL
file(GLOB_RECURSE SOURCE_FILES_CORE src/core/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_RENDER src/render/*.cpp)
list(APPEND SOURCE_FILES ${SOURCE_FILES_CORE} ${SOURCE_FILES_RENDER})
if(CSUGL_BUILD_MODEL_LOADER)
    list(APPEND SOURCE_FILES src/utils/model_loader.cpp)
endif()

file(GLOB_RECURSE HEADER_FILES_CORE src/core/*.h)
file(GLOB_RECURSE HEADER_FILES_RENDER src/render/*.h)
list(APPEND HEADER_FILES ${HEADER_FILES_CORE} ${HEADER_FILES_RENDER})
list(APPEND HEADER_FILES src/utils/asset_manager.h src/utils/singleton.h src/utils/thread_pool.h)
if(CSUGL_BUILD_MODEL_LOADER)
    list(APPEND HEADER_FILES src/utils/model_loader.h)
endif()

file(GLOB VENDOR_FILES ${GLAD_SRC})

source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})
source_group("Vendors" FILES ${VENDOR_FILES})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message("Use MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else ()
    if (NOT WIN32)
        LIST(APPEND LIBS dl)
    endif ()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message("Use Gnu")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -O3 -Wa,-mbig-obj -std=gnu++17")
    else()
        message("Use Other")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    endif()
endif ()

# add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${VENDOR_FILES})
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${VENDOR_FILES})
include_directories(src)
target_link_libraries(${PROJECT_NAME} ${LIBS})