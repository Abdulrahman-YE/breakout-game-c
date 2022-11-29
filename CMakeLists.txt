cmake_minimum_required(VERSION 3.18)

project(
  breakout
  VERSION "0.1.0"
  DESCRIPTION "Breakout game implemented in C language"
  LANGUAGES C)

include(FetchContent)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL2_DISABLE_SDL2MAIN OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.24.2)
FetchContent_MakeAvailable(sdl)

add_executable(breakout
    list.c
    block.c
    vector.c
    window.c
    main.c
)

target_link_directories(breakout PRIVATE ${sdl_BINARY_DIR})
target_include_directories(breakout PRIVATE ${sdl_SOURCE_DIR}/include)

target_link_libraries(breakout PRIVATE SDL2d m pthread dl)