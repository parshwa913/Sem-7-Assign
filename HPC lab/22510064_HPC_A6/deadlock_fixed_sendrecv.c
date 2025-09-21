#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define BIGSIZE 10000000   // 10 million ints ~ 40 MB

int main(int argc, char* argv[]) {
    int rank, size;
    int *sendbuf = NULL;
    int *recvbuf = NULL;

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

    sendbuf = (int*)malloc(BIGSIZE * sizeof(int));
    recvbuf = (int*)malloc(BIGSIZE * sizeof(int));

    for (int i = 0; i < BIGSIZE; i++) {
        sendbuf[i] = rank;   // fill with process rank
    }

    // Safe exchange using MPI_Sendrecv
    MPI_Sendrecv(sendbuf, BIGSIZE, MPI_INT, 1 - rank, 0,
                 recvbuf, BIGSIZE, MPI_INT, 1 - rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d successfully exchanged data with process %d\n", 
           rank, 1 - rank);

    free(sendbuf);
    free(recvbuf);

    MPI_Finalize();
    return 0;
}
