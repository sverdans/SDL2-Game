cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROJECT_NAME 2048)

project(${PROJECT_NAME} LANGUAGES CXX)

if(NOT PKG_CONFIG_FOUND)
    include(CMakeFindDependencyMacro)
    find_dependency(PkgConfig)
endif()

function(fetch_dependicies)
    include(FetchContent)

    message(STATUS "Fetching Deps started")

    set(LIBS_DIR ${CMAKE_BINARY_DIR}/external)

    FetchContent_Declare(SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        SOURCE_DIR ${LIBS_DIR}/SDL
        GIT_TAG SDL2)
    FetchContent_MakeAvailable(SDL)
 
    FetchContent_Declare(SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        SOURCE_DIR ${LIBS_DIR}/SDL_image
        GIT_TAG SDL2)
    FetchContent_MakeAvailable(SDL_image)

    message(STATUS "Fetching Deps done")
endfunction()

fetch_dependicies()

file(GLOB_RECURSE SOURCE "source/*.cpp")
file(GLOB_RECURSE INCLUDE "include/*.h")

add_executable(${PROJECT_NAME} ${INCLUDE} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${LIBS_DIR}/SDL/include
    ${LIBS_DIR}/SDL_image)
    
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2::image)
    