cmake_minimum_required(VERSION 3.5)
# Set the project name
project (multi_thread_avx)
# source file
set(SOURCES
    avx_test.cpp 
    matrix_mul_avx.cpp
    rand_matrix.cpp
)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#compile options
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -fno-tree-vectorize")
# target_compile_options(multi_thread_avx
# 	PRIVATE -O3 -march=native -fno-tree-vectorize
# )

# Add an executable
add_executable(multi_thread_avx ${SOURCES})




