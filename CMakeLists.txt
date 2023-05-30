cmake_minimum_required(VERSION 3.16.3)
project(FEUP_DA_Proj2)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/graph)

set(PROJECT_HEADERS
        src/cli/Helpy.h
        lib/graph/src/implementation/Graph.h
        lib/graph/src/UGraph.h
        lib/libfort/fort.hpp
        src/utils/Reader.h
        src/utils/Utils.hpp)

set(PROJECT_SOURCES
        src/cli/Helpy.cpp
        lib/graph/src/implementation/Graph.cpp
        lib/graph/src/UGraph.cpp
        lib/libfort/fort.c
        src/main.cpp
        src/utils/Reader.cpp)

add_executable(DA-Proj2
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})

target_include_directories(DA-Proj2
        lib/graph/include)
