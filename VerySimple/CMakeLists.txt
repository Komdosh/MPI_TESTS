cmake_minimum_required(VERSION 3.12)
project(VerySimple)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

SET(CUSTOM_BUILD_MPICH_FOLDER /Users/komdosh/Projects/University/MPIBuild/build)

SET(MPI_PER_GLOBAL ${CUSTOM_BUILD_MPICH_FOLDER}/mpich4-global/bin/)
SET(MPI_PER_VNI ${CUSTOM_BUILD_MPICH_FOLDER}/mpich4-pervni/bin/)

SET(CURRENT_MPI ${MPI_PER_GLOBAL})
SET(MPICH_SRC /Users/komdosh/Projects/University/MPIBuild/mpich-3.3/src)

include_directories(${MPICH_SRC} ${MPICH_SRC}/include ${CURRENT_MPI})

SET(CMAKE_C_COMPILER ${CURRENT_MPI}mpicc)
SET(CMAKE_CXX_COMPILER ${CURRENT_MPI}mpic++)

SET(SOURCE_FILES main.cpp)
add_executable(VerySimple ${SOURCE_FILES})

target_link_libraries(VerySimple ${MPI_LIBRARIES})

