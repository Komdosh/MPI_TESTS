cmake_minimum_required(VERSION 3.12)
project(MPIThreads)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

SET(MPI_PER_GLOBAL /Users/komdosh/Projects/University/MPIBuild/build/mpich4-global/bin/)
SET(MPI_PER_VNI /Users/komdosh/Projects/University/MPIBuild/build/mpich4-pervni/bin/)

SET(CURRENT_MPI )

SET(CMAKE_C_COMPILER ${CURRENT_MPI}mpicc)
SET(CMAKE_CXX_COMPILER ${CURRENT_MPI}mpic++)

SET(SOURCE_FILES main.cpp)
add_executable(MPIThreads ${SOURCE_FILES})

target_link_libraries(MPIThreads ${MPI_LIBRARIES})
