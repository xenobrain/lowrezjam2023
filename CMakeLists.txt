cmake_minimum_required(VERSION 3.25)
project(HyperspaceRescue VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)


# Install CPM ##########################################################################################################
set(CPM_VERSION 0.38.1)
set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/libraries)
set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_VERSION}/CPM.cmake
            ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})


# Install Libraries ####################################################################################################
CPMAddPackage("gh:nothings/stb#5736b15f7ea0ffb08dd38af21067c314d6a3aae9")
CPMAddPackage("gh:libsdl-org/SDL#release-2.28.2")
CPMAddPackage("gh:freetype/freetype#VER-2-13-0")
CPMAddPackage("gh:mackron/miniaudio#0.11.18")

add_library(Freetype::Freetype ALIAS freetype)

set(COMMON_INCLUDES source)


# Build Engine Library #################################################################################################
file(GLOB_RECURSE engine_HEADERS source/engine/*.h)
file(GLOB_RECURSE engine_SOURCES source/engine/*.cpp)
add_library(engine ${engine_HEADERS} ${engine_SOURCES})
target_include_directories(engine PRIVATE ${COMMON_INCLUDES} ${stb_SOURCE_DIR} ${miniaudio_SOURCE_DIR})
target_link_libraries(engine PRIVATE SDL2::SDL2-static freetype)


# Build Game Executable ################################################################################################
file(GLOB_RECURSE game_HEADERS source/game/*.h)
file(GLOB_RECURSE game_SOURCES source/game/*.cpp)
add_executable(HyperspaceRescue ${game_HEADERS} ${game_SOURCES})
target_include_directories(HyperspaceRescue PRIVATE ${COMMON_INCLUDES} ${stb_SOURCE_DIR})
target_link_libraries(HyperspaceRescue PRIVATE engine)

