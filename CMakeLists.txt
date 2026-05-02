cmake_minimum_required(VERSION 3.20)

project(AiZO_Projekt2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

include_directories(structures)
include_directories(algorithms)
include_directories(research)
include_directories(lib)

add_executable(AiZO_Projekt2
        main.cpp
        structures/Edge.cpp
        structures/MatrixGraph.cpp
        structures/ListGraph.cpp
        structures/FileLoader.cpp
        structures/IGraph.h
)