cmake_minimum_required(VERSION 3.12)
project(MPIRMA)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 14)

add_executable(MPIRMA main.cpp)

target_link_libraries(MPIRMA ${MPI_LIBRARIES})
