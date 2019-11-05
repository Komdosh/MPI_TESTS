cmake_minimum_required(VERSION 3.10)
project(VerySimple)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

SET(CURRENT_MPI /home/atabakov/projects/MPICH_DEV/mpich/compiled/global)
#SET(MPICH_SRC ../../MPI_ETU/mpich/mpich-3.3/src)

#include_directories(${MPICH_SRC} ${MPICH_SRC}/include ${CURRENT_MPI} /usr/src/glibc/debian/sysdeps)
include_directories(${CURRENT_MPI}/include ${CURRENT_MPI})

SET(CMAKE_C_COMPILER ${CURRENT_MPI}/bin/mpicc)
SET(CMAKE_CXX_COMPILER ${CURRENT_MPI}/bin/mpic++)

SET(SOURCE_FILES main.cpp)
add_executable(VerySimple ${SOURCE_FILES})

target_link_libraries(VerySimple ${MPI_LIBRARIES})

