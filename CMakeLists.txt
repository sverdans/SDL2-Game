cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(BUILD_SHARED_LIBS OFF)
set(PROJECT_NAME 2048)
set(BINARY ${PROJECT_NAME}.nostrip.exe)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

project(${PROJECT_NAME} LANGUAGES CXX)

if(NOT PKG_CONFIG_FOUND)
    include(CMakeFindDependencyMacro)
    find_dependency(PkgConfig)
endif()

function(fetch_dependicies)
    message(STATUS "Fetching project dependency - start")

    include(FetchContent)

    set(LIBS_DIR ${CMAKE_BINARY_DIR}/external)

    set(SDL_STATIC ON)
    set(SDL_SHARED OFF)
    FetchContent_Declare(SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        SOURCE_DIR ${LIBS_DIR}/SDL
        GIT_TAG SDL2)
    FetchContent_MakeAvailable(SDL)
    include_directories(${LIBS_DIR}/SDL/include)

    FetchContent_Declare(SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        SOURCE_DIR ${LIBS_DIR}/SDL_image
        GIT_TAG SDL2)
    FetchContent_MakeAvailable(SDL_image)
    include_directories(${LIBS_DIR}/SDL_image)

    message(STATUS "Fetching project dependency - done")
endfunction()

fetch_dependicies()

file(GLOB_RECURSE SOURCE "source/*.cpp")
file(GLOB_RECURSE INCLUDE "include/*.h")

add_executable(${BINARY} ${INCLUDE} ${SOURCE})

target_include_directories(${BINARY}
    PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${BINARY}
    PRIVATE SDL2::SDL2
            SDL2_image::SDL2_image-static)

add_custom_command(TARGET ${BINARY} POST_BUILD
    COMMAND strip -o ${PROJECT_NAME}.strip.exe -s ${BINARY}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
