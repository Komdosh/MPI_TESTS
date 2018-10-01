#include <iostream>
#include <math.h>
#include <x86intrin.h>
#include <zconf.h>
#include "mpi.h"
#include "cpu_helper.cpp"

#define NUM_ELEMENT  2
#define CORES 6
#define NUM_OF_THREADS 2

using namespace std;


struct threadData {
    int threadId;
    int rank;
    int *shared;
    MPI_Win win;
};

void printArray(const int *array) {
    for (int i = 0; i < NUM_ELEMENT; i++)
        printf(" %d", array[i]);

    printf("\n");
}

void putSharedArr(MPI_Win win, int *arrToSend, int threadId) {
    int rankToSend = 1;

    printf("Put data in the shared memory of rank %d, thread %d\n", rankToSend, threadId);
    MPI_Put(&(arrToSend[0]), NUM_ELEMENT, MPI_INT, rankToSend, threadId, NUM_ELEMENT, MPI_INT, win);
}

void getSharedArr(MPI_Win win, int rank, int threadId) {
    int local[NUM_ELEMENT] = {0};
    MPI_Get(&(local[0]), NUM_ELEMENT, MPI_INT, rank, threadId, NUM_ELEMENT, MPI_INT, win);

    printf("Rank %d, thread %d gets data from the shared memory:", rank, threadId);
    printArray(local);
}

void sendSharedArr(int *arrToSend, int threadId){
    int rankToSend = 1;

    printf("Send data to rank %d, thread %d\n", rankToSend, threadId);
    MPI_Send(&(arrToSend[0]), NUM_ELEMENT, MPI_INT, rankToSend, threadId, MPI_COMM_WORLD);
}

void receiveSharedArr(int rank, int threadId){
    int sourceRank = 0;

    int local[NUM_ELEMENT] = {0};

    MPI_Recv(&(local[0]), NUM_ELEMENT, MPI_INT, sourceRank, threadId, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Rank %d, thread %d receive data", rank, threadId);
    printArray(local);
}

void *MPIAction(void *threadarg) {
    struct threadData *threadData = (struct threadData *) threadarg;
    int rank = threadData->rank;
    int threadId = threadData->threadId;
    int *shared = threadData->shared;
    MPI_Win win = threadData->win;

    //printf("rank %d, thread %d, %p\n", rank, threadId, shared);
    if (rank == 0) {
        for (int i = 0; i < NUM_ELEMENT; i++) {
            shared[i] = (rank+1)*100+(threadId+1)*10+i;
        }
        printf("Rank %d, thread %d sets data in the shared memory:", rank, threadId);

        printArray(shared);
    }

    MPI_Win_fence(0, win);
    if (rank == 0) {
        putSharedArr(win, shared, threadId);
    }
    MPI_Win_fence(0, win);
    if (rank == 1) {
        getSharedArr(win, rank, threadId);
    }
    MPI_Win_fence(0, win);

    if (rank == 0) {
        for (int i = 0; i < NUM_ELEMENT; i++) {
            shared[i] += NUM_ELEMENT;
        }
        printf("Rank %d, thread %d sets data in the shared memory:", rank, threadId);

        printArray(shared);
    }


    if (rank == 0) {
        sendSharedArr(shared, threadId);
    }

    if (rank == 1) {
        receiveSharedArr(rank, threadId);
    }

    pthread_exit(nullptr);
}

mutex m;

int main(int argc, char **argv) {

    cpu_set_t cpuset[CORES];
    pthread_t threads[NUM_OF_THREADS];
    for (int i = 0; i < CORES; i++) {
        CPU_ZERO(&cpuset[i]);
        CPU_SET(i, &cpuset[i]);
    }

    int rank, np, len;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Get_processor_name(processorName, &len);

    if (np != 2) {
        printf("Number of process is %d, you should run this program with 2 process\n", np);
        return -1;
    }

    //printf("Rank %d running on %s\n", rank, processorName);

    MPI_Win win[NUM_OF_THREADS];
    int shared[NUM_OF_THREADS][NUM_ELEMENT] = {{0}, {0}};
    struct threadData td[NUM_OF_THREADS];

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        td[i].rank = rank;
        td[i].threadId = i;
        td[i].shared = shared[i];
        printf("rank %d, thread %d, %p\n", rank, i, shared[i]);
        MPI_Win_create(shared[i], NUM_ELEMENT, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &(win[i]));
        td[i].win = win[i];
        int rc = pthread_create(&threads[i], nullptr, MPIAction, (void *) &td[i]);

        int s = pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset[i % CORES]);
        if (s != 0) {
            printf("Thread %d affinities was not set", i);
            pthread_exit(nullptr);
        }

        if (rc) {
            cout << "Error: thread wasn't created," << rc << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    for (int i = 0; i < NUM_OF_THREADS && rank == 0; i++) {
        MPI_Win_free(&win[i]);
    }
    MPI_Finalize();
    return 0;
}
