cmake_minimum_required(VERSION 3.6)
project(semestralka)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp pacman.h tiles.h map.h abc.cpp tiles.cpp pacman.cpp map.cpp ghosts.cpp ghosts.h exception.cpp exception.h)
add_executable(semestralka ${SOURCE_FILES})