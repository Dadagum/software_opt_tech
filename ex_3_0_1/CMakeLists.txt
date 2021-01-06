cmake_minimum_required(VERSION 3.5)
# Set the project name
project (final_subject)
# source file
set(SOURCES
    scan_like_operations.cpp 
    search.cpp
    test.cpp
)

#compile options
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -fno-tree-vectorize")
# set (CMAKE_CXX_FLAGS "-march=native -ggdb")

# Add an executable
add_executable(db_opt ${SOURCES})




