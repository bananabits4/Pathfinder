cmake_minimum_required(VERSION 3.14)
project(pathfinder CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pathfinder
    main.cpp
    graph.cpp
    search.cpp
    json_export.cpp
)

target_compile_options(pathfinder PRIVATE -Wall -Wextra -O2)
