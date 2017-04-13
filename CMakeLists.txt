cmake_minimum_required(VERSION 3.6)
project(GA)

set(CMAKE_CXX_STANDARD 14)
SET(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#SET(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt5Core")
include_directories(/home/dumitru/Qt/5.4/gcc_64/include/)
link_directories(/home/dumitru/Qt/5.4/gcc_64/lib/)

#find_package(Qt5 COMPONENTS QtCore QtNetwork REQUIRED)
#find_package(QtCore)
set(CMAKE_VERBOSE_MAKEFILE on)

set (CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,/home/dumitru/Qt/5.4/gcc_64/lib")

include_directories(include)
set(SOURCE_FILES
        main.cpp
        )

add_executable(GA ${SOURCE_FILES})

#qt5_use_modules(GA QtCore)
target_link_libraries(GA Qt5Network)
target_link_libraries(GA Qt5Core)

