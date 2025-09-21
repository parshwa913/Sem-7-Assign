#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int color = (world_rank < (world_size+1)/2) ? 0 : 1;
    MPI_Comm newcomm;
    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &newcomm);

    int new_rank, new_size;
    MPI_Comm_rank(newcomm, &new_rank);
    MPI_Comm_size(newcomm, &new_size);

    printf("World rank %d/%d -> color=%d -> newcomm rank %d/%d\n",
           world_rank, world_size, color, new_rank, new_size);

    MPI_Comm_free(&newcomm);
    MPI_Finalize();
    return 0;
}
