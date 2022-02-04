cmake_minimum_required(VERSION 3.0)

project(cecie)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(BEFORE SYSTEM $ENV{OO_PS4_TOOLCHAIN}/include)
include_directories(${root_SOURCE_DIR}/include)

set(SOURCES source/main.cpp)

add_executable(cecie.out ${SOURCES})
