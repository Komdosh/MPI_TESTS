#include <iostream>
#include <x86intrin.h>
#include <mpi.h>
#include <unistd.h>
#include "cpu_helper.cpp"

int ELEMENTS = 50000;
int CORES = 4;
int NUM_OF_THREADS = 3;
int RATE = 10000;

using namespace std;

struct ThreadData_s {
    int threadId;
    int rank;
    int *shared;
    double *times;
};

typedef ThreadData_s ThreadData;

pthread_barrier_t pThreadBarrier;

void sendSharedArr(int *arrToSend, int destRank, int tag) {
    for (int i = 0; i < RATE; ++i) {
        MPI_Send(&(arrToSend[0]), ELEMENTS, MPI_INT, destRank, tag, MPI_COMM_WORLD);
//    printf("SEND: destRank: %d, tag: %d\n", destRank, tag);
    }
}

void receiveSharedArr(int sourceRank, int tag) {
    int *local = new int[ELEMENTS];
    for (int i = 0; i < RATE; ++i) {
        MPI_Recv(&(local[0]), ELEMENTS, MPI_INT, sourceRank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void *MPIAction(void *threadarg) {
    auto *threadData = (ThreadData *) threadarg;
    int rank = threadData->rank;
    int threadId = threadData->threadId;
    int *shared = threadData->shared;
    double *times = threadData->times;

    double start = MPI_Wtime();
    sendSharedArr(shared, threadId + 1, threadId + 1);

    pthread_barrier_wait(&pThreadBarrier);
    if (threadId == NUM_OF_THREADS - 1) {
        MPI_Barrier(MPI_COMM_WORLD);
    }
//    printf("MAIN RECV: sourceRank: %d, tag: %d\n", threadId+1, threadId+1);
    receiveSharedArr(threadId + 1, threadId + 1);

    times[threadId] = MPI_Wtime() - start;
    pthread_exit(nullptr);
}

void runMPI(char **argv) {
    int provided;
    int rank, np, len;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    int argc = 1;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Get_processor_name(processorName, &len);

    double times[NUM_OF_THREADS];

    if (rank == 0) {
        pthread_barrier_init(&pThreadBarrier, nullptr, NUM_OF_THREADS);
        cpu_set_t cpuset[CORES];
        pthread_t threads[NUM_OF_THREADS];
        for (int i = 0; i < CORES; i++) {
            CPU_ZERO(&cpuset[i]);
            CPU_SET(i, &cpuset[i]);
        }

        int shared[NUM_OF_THREADS][ELEMENTS];

        ThreadData td[NUM_OF_THREADS];

        for (int i = 0; i < NUM_OF_THREADS; i++) {
            td[i].rank = rank;
            td[i].threadId = i;
            td[i].shared = shared[i];
            td[i].times = times;

            for (int n = 0; n < ELEMENTS; n++) {
                shared[i][n] = (rank + 1) * 100 + (i + 1) * 10 + n;
            }

            int rc = pthread_create(&threads[i], nullptr, MPIAction, (void *) &td[i]);

            int s = pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset[i % CORES]);
            if (s != 0) {
                pthread_exit(nullptr);
            }

            if (rc) {
                exit(-1);
            }
        }
        double maxTime = 0.0;
        for (int i = 0; i < NUM_OF_THREADS; i++) {
            pthread_join(threads[i], nullptr);
            if (maxTime < times[i]) {
                maxTime = times[i];
            }
        }
        pthread_barrier_destroy(&pThreadBarrier);
        printf("%d\n", (int) (maxTime * 1000));
    } else {
        int *shared = new int[ELEMENTS]{0};
        for (int n = 0; n < ELEMENTS; n++) {
            shared[n] = (rank + 1) * 100 + (rank + 1) * 10 + n;
        }
//        printf("CHILD RECV: sourceRank: %d, tag: %d\n", 0, rank);
        MPI_Barrier(MPI_COMM_WORLD);
        receiveSharedArr(0, rank);
        sendSharedArr(shared, 0, rank);
    }

    MPI_Finalize();
}

int main(int argc, char **argv) {
    NUM_OF_THREADS = atoi(argv[1]) - 1;
    CORES = atoi(argv[2]);
    RATE = atoi(argv[3]);
    ELEMENTS = atoi(argv[4]);

    if (NUM_OF_THREADS < 2) {
        printf("Threads number should be greater than 2\n");
        exit(1);
    }
    char *args[] = {
            (char *) "VerySimple",
            NULL
    };
    runMPI(args);

    return 0;
}
