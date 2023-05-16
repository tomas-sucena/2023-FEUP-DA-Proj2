cmake_minimum_required(VERSION 3.24)
project(FEUP_DA_Proj2)

set(CMAKE_CXX_STANDARD 14)

include_directories(graph/src)
include_directories(graph/src/implementation)
include_directories(src)
include_directories(src/libfort)

add_executable(FEUP_DA_Proj2
        graph/src/implementation/DynamicPQ.hpp
        graph/src/implementation/Edge.hpp
        graph/src/implementation/Graph.cpp
        graph/src/implementation/Graph.h
        graph/src/implementation/Path.hpp
        graph/src/implementation/Vertex.hpp
        graph/src/DGraph.cpp
        graph/src/DGraph.h
        graph/src/UGraph.cpp
        graph/src/UGraph.h
        src/libfort/fort.c
        src/libfort/fort.h
        src/libfort/fort.hpp
        src/Helpy.cpp
        src/Helpy.h
        src/main.cpp
        src/Utils.hpp src/Reader.cpp src/Reader.h)
