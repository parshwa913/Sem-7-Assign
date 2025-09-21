#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int my_value;
    if (rank == 0) {
        printf("Enter an integer value (rank 0): ");
        fflush(stdout);
        scanf("%d", &my_value);
    }
    MPI_Bcast(&my_value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int left = (rank - 1 + size) % size;
    int right = (rank + 1) % size;
    int recv_value = -1;

    if (rank % 2 == 0) {
        MPI_Send(&my_value, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_value, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&recv_value, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&my_value, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
    }

    printf("Rank %d sent %d to %d, received %d from %d\n",
           rank, my_value, right, recv_value, left);

    MPI_Finalize();
    return 0;
}
