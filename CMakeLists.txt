cmake_minimum_required(VERSION 3.24)
project(FEUP_DA_Proj2)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(graph)
include_directories(graph/src)

set(PROJECT_HEADERS
        graph/src/implementation/Graph.h
        graph/src/UGraph.h
        src/Helpy.h
        src/libfort/fort.hpp
        src/Reader.h
        src/Utils.hpp)

set(PROJECT_SOURCES
        graph/src/implementation/Graph.cpp
        graph/src/UGraph.cpp
        src/Helpy.cpp
        src/libfort/fort.c
        src/main.cpp
        src/Reader.cpp)

add_executable(DA-Proj2
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})
