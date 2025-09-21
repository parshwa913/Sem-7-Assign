#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) printf("Run this program with exactly 2 processes!\n");
        MPI_Finalize();
        return 0;
    }

    int n;
    int *arr = NULL;

    if (rank == 0) {
        printf("Enter total number of elements: ");
        fflush(stdout);
        scanf("%d", &n);
        arr = (int*)malloc(n * sizeof(int));
        printf("Enter %d integers: ", n);
        for (int i = 0; i < n; i++) scanf("%d", &arr[i]);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int half = n / 2;
    int remaining = n - half;
    int local_sum = 0;

    if (rank == 0) {
        for (int i = 0; i < half; i++) local_sum += arr[i];
        MPI_Send(arr + half, remaining, MPI_INT, 1, 0, MPI_COMM_WORLD);
        int sum1;
        MPI_Recv(&sum1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Final sum = %d (P0=%d + P1=%d)\n", local_sum + sum1, local_sum, sum1);
        free(arr);
    } else if (rank == 1) {
        int *subarr = (int*)malloc(remaining * sizeof(int));
        MPI_Recv(subarr, remaining, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < remaining; i++) local_sum += subarr[i];
        MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        free(subarr);
    }

    MPI_Finalize();
    return 0;
}
