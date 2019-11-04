#!/usr/bin/env sh

LOG_PREFIX="[Run MPICH]:"

MPICH_MPIEXEC="/bin/mpiexec"
#MPICH_MPICC="/bin/mpicc"
#MPICH_MPIC++="/bin/mpicc"


PROGRAM_PATH="cmake-build-debug/VerySimple"

NUMBER_OF_PROCESS="2"

setDebug(){
  if test -f "log/dbg*"; then
    rm "log/dbg*"
  fi
  echo "${LOG_PREFIX} activate debug mode"
  export MPICH_DBG_FILENAME="log/dbg-%d.log"
  export MPICH_DBG_CLASS="ALL"
  export MPICH_DBG_LEVEL="VERBOSE"
}

CURRENT_MPI=""

while getopts ":m:dn:" opt; do
	case $opt in
		m) #mpi path
			CURRENT_MPI="${OPTARG}"
			;;
		n) #number of process
			NUMBER_OF_PROCESS="${OPTARG}"
			;;
		d)
		  setDebug
		  ;;
		*) #empty or unknown arg
			#do nothing
			;;
    esac
done

RUN_SCRIPT="${MPICH_DIR}${CURRENT_MPI}${MPICH_MPIEXEC} -host localhost -n ${NUMBER_OF_PROCESS} ${PROGRAM_PATH}"
echo "$LOG_PREFIX $RUN_SCRIPT"
eval "$RUN_SCRIPT"
