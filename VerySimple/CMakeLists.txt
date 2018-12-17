cmake_minimum_required(VERSION 3.12)
project(VerySimple)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpic++)

SET(SOURCE_FILES main.cpp)
add_executable(VerySimple ${SOURCE_FILES})

target_link_libraries(VerySimple ${MPI_LIBRARIES})

