cmake_minimum_required(VERSION 3.6)
project(GA)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(include)
set(SOURCE_FILES
        main.cpp
        src/GA.cpp
        src/Gene.cpp
        src/Nucleotide.cpp
        src/Organism.cpp
        src/OrganismFactory.cpp
        src/bit_ops.cpp
        src/Chromosome.cpp
        )

add_executable(GA ${SOURCE_FILES})