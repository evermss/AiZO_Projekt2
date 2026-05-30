cmake_minimum_required(VERSION 3.20)

project(AiZO_Projekt2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AiZO_Projekt2
        main.cpp
        structures/Edge.cpp
        structures/MatrixGraph.cpp
        structures/ListGraph.cpp
        structures/FileLoader.cpp
        algorithms/Prim.cpp
        algorithms/Kruskal.cpp
        algorithms/Dijkstra.cpp
        algorithms/BellmanFord.cpp
        algorithms/FordFulkerson.cpp
        research/Timer.cpp
        research/GraphGenerator.cpp
       


)

target_include_directories(AiZO_Projekt2 PRIVATE
        include
        structures
        algorithms
        research
)

target_link_directories(AiZO_Projekt2 PRIVATE
        lib
)

target_link_libraries(AiZO_Projekt2 PRIVATE
        parameters
)

target_compile_options(AiZO_Projekt2 PRIVATE
        -Wall
        -Wextra
        -Werror
)

set_target_properties(AiZO_Projekt2 PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/lib"
)