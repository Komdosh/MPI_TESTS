cmake_minimum_required(VERSION 3.12)
project(MPIThreads)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(MPIThreads main.cpp)

target_link_libraries(MPIThreads ${MPI_LIBRARIES})
