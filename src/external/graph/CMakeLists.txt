cmake_minimum_required(VERSION 3.16.3)
project(graph)

if (${CMAKE_VERSION} VERSION_GREATER "3.23.2")
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS
        src/DGraph.h
        src/implementation/DynamicPQ.hpp
        src/implementation/Edge.hpp
        src/implementation/Graph.h
        src/implementation/Path.hpp
        src/implementation/Vertex.hpp
        src/UGraph.h
        tests/TestGraphs.h)

set(PROJECT_SOURCES
        src/DGraph.cpp
        src/implementation/Graph.cpp
        src/UGraph.cpp
        tests/TestGraphs.cpp)

set(PROJECT_TESTS
        tests/bfsTest.cpp
        tests/dfsTest.cpp
        tests/dijkstraTest.cpp
        tests/dynamicPQTest.cpp
        tests/initializationTest.cpp
        tests/manipulationTest.cpp
        tests/maximumFlowTest.cpp)

add_executable(graph
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        ${PROJECT_TESTS})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(graph
        gtest
        gtest_main)

enable_testing()
