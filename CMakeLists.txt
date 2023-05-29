cmake_minimum_required(VERSION 3.16.3)
project(FEUP_DA_Proj2)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src/external/graph)

set(PROJECT_HEADERS
        src/cli/Helpy.h
        src/external/graph/src/implementation/Graph.h
        src/external/graph/src/UGraph.h
        src/external/libfort/fort.hpp
        src/utils/Reader.h
        src/utils/Utils.hpp)

set(PROJECT_SOURCES
        src/cli/Helpy.cpp
        src/external/graph/src/implementation/Graph.cpp
        src/external/graph/src/UGraph.cpp
        src/external/libfort/fort.c
        src/main.cpp
        src/utils/Reader.cpp)

add_executable(DA-Proj2
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})
