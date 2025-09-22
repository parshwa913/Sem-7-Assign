#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int n;
    int *matrix = NULL, *vector = NULL, *result = NULL;
    int *local_matrix, *local_result;
    int rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter size of square matrix (n): ");
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
        matrix = (int*)malloc(n * n * sizeof(int));
        vector = (int*)malloc(n * sizeof(int));
        result = (int*)malloc(n * sizeof(int));

        printf("Enter matrix (%d x %d):\n", n, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &matrix[i * n + j]);

        printf("Enter vector (%d elements):\n", n);
        for (int i = 0; i < n; i++)
            scanf("%d", &vector[i]);
    }

    local_matrix = (int*)malloc(rows_per_proc * n * sizeof(int));
    local_result = (int*)malloc(rows_per_proc * sizeof(int));
    if (rank != 0) vector = (int*)malloc(n * sizeof(int));

    MPI_Scatter(matrix, rows_per_proc * n, MPI_INT,
                local_matrix, rows_per_proc * n, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Bcast(vector, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_proc; i++) {
        local_result[i] = 0;
        for (int j = 0; j < n; j++) {
            local_result[i] += local_matrix[i * n + j] * vector[j];
        }
    }

    MPI_Gather(local_result, rows_per_proc, MPI_INT,
               result, rows_per_proc, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result vector:\n");
        for (int i = 0; i < n; i++)
            printf("%d ", result[i]);
        printf("\n");
    }

    if (rank == 0) { free(matrix); free(vector); free(result); }
    else free(vector);
    free(local_matrix);
    free(local_result);

    MPI_Finalize();
    return 0;
}
