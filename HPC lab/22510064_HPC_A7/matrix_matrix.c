#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int n;
    int *A = NULL, *B = NULL, *C = NULL;
    int *local_A, *local_C;
    int rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter size of square matrices (n): ");
        fflush(stdout);
        scanf("%d", &n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n % size != 0) {
        if (rank == 0) {
            printf("Error: n (%d) must be divisible by number of processes (%d)\n", n, size);
        }
        MPI_Finalize();
        return 0;
    }

    rows_per_proc = n / size;

    if (rank == 0) {
        A = (int*)malloc(n * n * sizeof(int));
        B = (int*)malloc(n * n * sizeof(int));
        C = (int*)malloc(n * n * sizeof(int));

        printf("Enter matrix A (%d x %d):\n", n, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &A[i * n + j]);

        printf("Enter matrix B (%d x %d):\n", n, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &B[i * n + j]);
    }

    local_A = (int*)malloc(rows_per_proc * n * sizeof(int));
    local_C = (int*)malloc(rows_per_proc * n * sizeof(int));
    if (rank != 0) B = (int*)malloc(n * n * sizeof(int));

    MPI_Scatter(A, rows_per_proc * n, MPI_INT,
                local_A, rows_per_proc * n, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Bcast(B, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < n; j++) {
            local_C[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                local_C[i * n + j] += local_A[i * n + k] * B[k * n + j];
            }
        }
    }

    MPI_Gather(local_C, rows_per_proc * n, MPI_INT,
               C, rows_per_proc * n, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result matrix C:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%d ", C[i * n + j]);
            printf("\n");
        }
    }

    if (rank == 0) { free(A); free(B); free(C); }
    else free(B);
    free(local_A);
    free(local_C);

    MPI_Finalize();
    return 0;
}
