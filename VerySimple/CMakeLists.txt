cmake_minimum_required(VERSION 3.12)
project(VerySimple)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

SET(CUSTOM_BUILD_MPICH_FOLDER /opt/mpich)

SET(MPI_PER_GLOBAL ${CUSTOM_BUILD_MPICH_FOLDER}/global/bin)
SET(MPI_PER_VNI_HANDOFF ${CUSTOM_BUILD_MPICH_FOLDER}/per-vni-handoff/bin)
SET(MPI_PER_VNI_TRYLOCK ${CUSTOM_BUILD_MPICH_FOLDER}/per-vni-trylock/bin)

SET(CURRENT_MPI ${MPI_PER_VNI_TRYLOCK})
SET(MPICH_SRC ../../MPI_ETU/mpich/mpich-3.3/src)

include_directories(${MPICH_SRC} ${MPICH_SRC}/include ${CURRENT_MPI} /usr/src/glibc/debian/sysdeps)


SET(CMAKE_C_COMPILER ${CURRENT_MPI}/mpicc)
SET(CMAKE_CXX_COMPILER ${CURRENT_MPI}/mpic++)

SET(SOURCE_FILES main.cpp)
add_executable(VerySimple ${SOURCE_FILES})

target_link_libraries(VerySimple ${MPI_LIBRARIES})

