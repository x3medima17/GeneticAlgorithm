cmake_minimum_required(VERSION 3.6)
project(GA)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(include)
set(SOURCE_FILES
        main.cpp
        )

add_executable(GA ${SOURCE_FILES})