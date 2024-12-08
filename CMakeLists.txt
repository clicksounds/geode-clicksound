cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Some optimization for size reduction
if (MSVC)
    # MSVC-specific flags for size optimization
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /DEBUG:NONE /Gw /GR- /EHsc /w")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS Dumbshit specific size optimization flags
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -ffunction-sections -fdata-sections -flto -fno-rtti -fno-exceptions -g0")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,-dead_strip -Wl,-x")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC/Clang-specific flags for size optimization
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -ffunction-sections -fdata-sections -flto -fno-rtti -fno-exceptions")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -Wl,--strip-all")
else()
    # Compiler is unknown
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -Wl,--strip-all")
endif()


project(ClickSoundsLite VERSION 1.0.0)

# Add Everything under src dir
file(GLOB_RECURSE SOURCES src/*.cpp)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})

if (EXISTS "${CMAKE_COMMAND}" AND COMMAND_UPX_AVAILABLE)
    # Strip using macos
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND strip -x "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMAND upx --best "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Stripping and compressing binary with UPX for macOS")
    else()
        # Normal Compress command
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND upx --best "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Compressing binary with UPX")
    endif()
endif()

