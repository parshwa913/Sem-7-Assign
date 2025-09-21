#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define BIGSIZE 10000000  
int main(int argc, char* argv[]) {
    int rank, size;
    int *buffer = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("Run this program with exactly 2 processes!\n");
        }
        MPI_Finalize();
        return 0;
    }

    buffer = (int*)malloc(BIGSIZE * sizeof(int));

    if (rank == 0) {
        MPI_Send(buffer, BIGSIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, BIGSIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 finished exchange\n");
    } else if (rank == 1) {
        MPI_Send(buffer, BIGSIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, BIGSIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 finished exchange\n");
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}
