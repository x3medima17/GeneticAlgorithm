cmake_minimum_required(VERSION 3.6)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp bit_ops.h Nucleotide.hpp Chromosome.hpp Gene.hpp GA.hpp Organism.hpp)
add_executable(untitled ${SOURCE_FILES})