cmake_minimum_required(VERSION 3.16.3)
project(FEUP_DA_Proj2)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/graph)

include_directories(lib
        lib/graph/include)

set(PROJECT_HEADERS
        src/cli/Helpy.h
        lib/graph/include/UGraph.h
        lib/libfort/fort.hpp
        src/utils/Reader.h
        src/utils/Utils.hpp)

set(PROJECT_SOURCES
        src/cli/Helpy.cpp
        lib/graph/src/UGraph.cpp
        lib/libfort/fort.c
        src/main.cpp
        src/utils/Reader.cpp)

add_executable(DA-Proj2
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})
