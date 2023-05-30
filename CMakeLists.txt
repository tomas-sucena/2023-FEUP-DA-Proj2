cmake_minimum_required(VERSION 3.16.3)
project(FEUP_DA_Proj2)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/graph)

include_directories(lib
        lib/graph/include)

set(PROJECT_HEADERS
        lib/graph/include/UGraph.h
        lib/graph/src/Graph.h
        lib/libfort/fort.hpp
        src/cli/Helpy.h
        src/network/DataGraph.h
        src/network/Place.hpp
        src/utils/Reader.h
        src/utils/Utils.hpp)

set(PROJECT_SOURCES
        lib/graph/src/UGraph.cpp
        lib/graph/src/Graph.cpp
        lib/libfort/fort.c
        src/cli/Helpy.cpp
        src/network/DataGraph.cpp
        src/main.cpp
        src/utils/Reader.cpp)

add_executable(DA_Proj2
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})
