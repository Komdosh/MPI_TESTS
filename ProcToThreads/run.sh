#!/usr/bin/env sh

LOG_PREFIX="[Run MPICH]:"

MPICH_MPIEXEC="/bin/mpiexec"
#MPICH_MPICC="/bin/mpicc"
#MPICH_MPIC++="/bin/mpic++"

PROGRAM_PATH="cmake-build-debug/VerySimple"

NUMBER_OF_PROCESS=2

CURRENT_MPI=""

CORES=4

while getopts ":m:dn:" opt; do
	case $opt in
		m) #mpi path
			CURRENT_MPI="${OPTARG}"
			;;
		n) #number of process
			NUMBER_OF_PROCESS="${OPTARG}"
			;;
	  d) #default script
	    for PROC_NUM in 3 4 5 6 7
      do
        sh ./run.sh -m /home/atabakov/projects/MPICH_DEV/mpich/compiled/multiqueue/global -n $PROC_NUM
      done
	    exit 0
	    ;;
		*) #empty or unknown arg
			#do nothing
			;;
    esac
done

RUN_SCRIPT="${MPICH_DIR}${CURRENT_MPI}${MPICH_MPIEXEC} -host localhost -n $NUMBER_OF_PROCESS $PROGRAM_PATH $NUMBER_OF_PROCESS $CORES"
#echo "$LOG_PREFIX $RUN_SCRIPT"
eval "$RUN_SCRIPT"
