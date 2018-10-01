#include <iostream>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define NUM_ELEMENT  4

void printArray(const int *array) {

    for (int i = 0; i < NUM_ELEMENT; i++)
        printf(" %d", array[i]);

    printf("\n");
}

void putSharedArr(MPI_Win win, int *arrToSend) {
    int rankToSend = 1;

    printf("Put data in the shared memory of rank %d\n", rankToSend);
    MPI_Put(&arrToSend[0], NUM_ELEMENT, MPI_INT, rankToSend, 0, NUM_ELEMENT, MPI_INT, win);
}

void getSharedArr(MPI_Win win, int rank) {
    int local[NUM_ELEMENT] = {0};

    MPI_Get(&local[0], NUM_ELEMENT, MPI_INT, rank, 0, NUM_ELEMENT, MPI_INT, win);

    printf("Rank %d gets data from the shared memory:", rank);
    printArray(local);
}

void sendSharedArr(int *arrToSend){
    int rankToSend = 1;

    printf("Send data to rank %d\n", rankToSend);
    MPI_Send(arrToSend, NUM_ELEMENT, MPI_INT, rankToSend, 0, MPI_COMM_WORLD);
}

void receiveSharedArr(int rank){
    int sourceRank = 0;

    int local[NUM_ELEMENT] = {0};

    MPI_Recv(&local[0], NUM_ELEMENT, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Rank %d receive data", rank);
    printArray(local);
}


int main(int argc, char **argv) {
    int i, rank, np, len, shared[NUM_ELEMENT];

    char processorName[MPI_MAX_PROCESSOR_NAME];
    MPI_Win win;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Get_processor_name(processorName, &len);

    if (np != 2) {
        printf("Number of process is %d, you should run this program with 2 process\n", np);
        return -1;
    }

    printf("Rank %d running on %s\n", rank, processorName);


    MPI_Win_create(shared, NUM_ELEMENT, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    if (rank == 0) {
        for (i = 0; i < NUM_ELEMENT; i++) {
            shared[i] = i;
        }
        printf("Rank %d sets data in the shared memory:", rank);

        printArray(shared);
    }


    MPI_Win_fence(0, win);
    if (rank == 0) {
        putSharedArr(win, shared);
    }

    MPI_Win_fence(0, win);
    if (rank == 1) {
        getSharedArr(win, rank);
    }

    MPI_Win_fence(0, win);

    MPI_Win_free(&win);

    if (rank == 0) {
        sendSharedArr(shared);
    }

    if (rank == 1) {
        receiveSharedArr(rank);
    }


    MPI_Finalize();
    return 0;
}
