#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int sizes[] = {100, 200, 300, 400, 500};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int i, j, t, s, N, M;
    double **A;
    double scalar = 5.0;
    double start_time, end_time, exec_time;
    FILE *fptr;

    fptr = fopen("PS_2.csv", "w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(fptr, "DataSize,Threads,Time(s)\n");

    printf("%-12s %-7s %s\n", "DataSize", "Threads", "Time(s)");
    printf("--------------------------------------------------\n");

    for (s = 0; s < num_sizes; s++) {
        N = sizes[s];
        M = sizes[s];

        A = (double **)malloc(N * sizeof(double *));
        for (i = 0; i < N; i++) {
            A[i] = (double *)malloc(M * sizeof(double));
        }

        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                A[i][j] = rand() % 100;
            }
        }

        for (t = 1; t <= 8; t *= 2) {
            omp_set_num_threads(t);

            start_time = omp_get_wtime();

            #pragma omp parallel for private(i, j) shared(A, scalar, N, M)
            for (i = 0; i < N; i++) {
                for (j = 0; j < M; j++) {
                    A[i][j] = A[i][j] * scalar;
                }
            }

            end_time = omp_get_wtime();
            exec_time = end_time - start_time;

            printf("%-12d %-7d %f\n", N*M, t, exec_time);
            fprintf(fptr, "%d,%d,%f\n", N*M, t, exec_time);
        }

        for (i = 0; i < N; i++) {
            free(A[i]);
        }
        free(A);
    }

    fclose(fptr);
    return 0;
}
