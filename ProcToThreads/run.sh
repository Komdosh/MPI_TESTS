#!/usr/bin/env sh

LOG_PREFIX="[Run MPICH]:"

MPICH_MPIEXEC="/bin/mpiexec"
#MPICH_MPICC="/bin/mpicc"
#MPICH_MPIC++="/bin/mpic++"

PROGRAM_PATH="cmake-build-debug/ProcToThreads"

NUMBER_OF_PROCESS=2

CURRENT_MPI=""

CORES=$(nproc)
RATE=20000
ELEMENTS=50000
MAX_PROC_NUM=6
REPEATS=10

while getopts ":m:dn:" opt; do
  case $opt in
  m) #mpi path
    CURRENT_MPI="${OPTARG}"
    ;;
  n) #number of process
    NUMBER_OF_PROCESS="${OPTARG}"
    ;;
  d) #default script
    for PROC_NUM in $(seq 3 $MAX_PROC_NUM); do
      TOTAL_TIME=0
      for REPEAT in $(seq 1 $REPEATS); do
        EXECUTION_TIME=$(sh ./run.sh -m /home/atabakov/projects/MPICH_DEV/mpich/compiled/per-vci-trylock -n $PROC_NUM)
        TOTAL_TIME=$((TOTAL_TIME + EXECUTION_TIME))
      done
      AVG_TIME=$((TOTAL_TIME / REPEATS))
      #      echo "AVG: $AVG_TIME TOTAL: $TOTAL_TIME"
      echo $((RATE * ELEMENTS * (PROC_NUM - 1) * 2 / AVG_TIME))
    done
    echo "MULTIQUEUE"
    for PROC_NUM in $(seq 3 $MAX_PROC_NUM); do
      TOTAL_TIME=0
      for REPEAT in $(seq 1 $REPEATS); do
        EXECUTION_TIME=$(sh ./run.sh -m /home/atabakov/projects/MPICH_DEV/mpich/compiled/multiqueue/per-vci-trylock -n $PROC_NUM)
        TOTAL_TIME=$((TOTAL_TIME + EXECUTION_TIME))
      done
      AVG_TIME=$((TOTAL_TIME / REPEATS))
      #      echo "AVG: $AVG_TIME TOTAL: $TOTAL_TIME"
      echo $((RATE * ELEMENTS * (PROC_NUM - 1) * 2 / AVG_TIME))

    done
    exit 0
    ;;
  *) #empty or unknown arg
    #do nothing
    ;;
  esac
done

RUN_SCRIPT="${MPICH_DIR}${CURRENT_MPI}${MPICH_MPIEXEC} -host localhost -n $NUMBER_OF_PROCESS $PROGRAM_PATH $NUMBER_OF_PROCESS $CORES $RATE $ELEMENTS"
#echo "$LOG_PREFIX $RUN_SCRIPT"
eval "$RUN_SCRIPT"
