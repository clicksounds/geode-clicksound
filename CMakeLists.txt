cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_C_FLAGS -m32)
set(CMAKE_CXX_FLAGS -m32)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(click-sound VERSION 1.0.0)

# Add Everything under src dir
file(GLOB SOURCES
	src/layers/*.cpp
    src/main.cpp
    src/ClickList.cpp
    src/ReleaseList.cpp
)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES})


if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Set up dependencies, resources, link Geode
setup_geode_mod(${PROJECT_NAME} EXTERNALS geode.node-ids:1.6.1)
