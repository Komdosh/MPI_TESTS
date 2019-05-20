#!/usr/bin/env sh

MPICH_DIR="/opt/mpich"
MPICH_TRYLOCK_DIR="/per-vni-trylock"
MPICH_HANDOFF_DIR="/per-vni-handoff"
MPICH_GLOBAL_DIR="/global"
MPICH_MPIEXEC="/bin/mpiexec"

PROGRAM_PATH="cmake-build-debug/VerySimple"

NUMBER_OF_PROCESS="2"


CURRENT_MPI=""

while getopts ":gp:n:" opt; do
	case $opt in
		g) #global critical section
      CURRENT_MPI="$MPICH_GLOBAL_DIR"
			;;
		p) #per-vni trylock
			perVniType=${OPTARG}
			echo $perVniType
      if test "$perVniType" != "trylock" && test "$perVniType" != "handoff"; then
			  echo $LOG_PREFIX "Wrong per-vni type, trylock or handoff only!"
			  exit 1
			fi
			CURRENT_MPI="/per-vni-$perVniType"
			;;
		n) #per-vni handoff
			NUMBER_OF_PROCESS="${OPTARG}"
			;;
		:) #empty or unknown arg
			#do nothing
			;;
    esac
done

eval "${MPICH_DIR}${CURRENT_MPI}${MPICH_MPIEXEC} -n ${NUMBER_OF_PROCESS} ${PROGRAM_PATH}"
