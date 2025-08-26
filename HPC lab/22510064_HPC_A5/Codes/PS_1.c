#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_THREADS 16   
#define MAX_SIZES 3      

int main() {
    int i, j, k, t, size_idx;
    double **A, **B, **C;
    double start_time, end_time;

    int sizes[MAX_SIZES] = {200, 400, 600};

    FILE *fp = fopen("PS_1.csv", "a");
    if (fp == NULL) {
        printf("Error opening PS_1.csv!\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "MatrixSize,Threads,ExecutionTime\n");
    }

    printf("%-12s %-7s %s\n", "DataSize", "Threads", "Time(s)");
    printf("--------------------------------------------------\n");

    for (size_idx = 0; size_idx < MAX_SIZES; size_idx++) {
        int N = sizes[size_idx];

        A = (double **)malloc(N * sizeof(double *));
        B = (double **)malloc(N * sizeof(double *));
        C = (double **)malloc(N * sizeof(double *));
        for (i = 0; i < N; i++) {
            A[i] = (double *)malloc(N * sizeof(double));
            B[i] = (double *)malloc(N * sizeof(double));
            C[i] = (double *)malloc(N * sizeof(double));
        }

        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = rand() % 100;
                B[i][j] = rand() % 100;
                C[i][j] = 0.0;
            }
        }

        for (t = 1; t <= MAX_THREADS; t *= 2) {  
            omp_set_num_threads(t);

            start_time = omp_get_wtime();

            #pragma omp parallel for private(i, j, k) shared(A, B, C)
            for (i = 0; i < N; i++) {
                for (j = 0; j < N; j++) {
                    double sum = 0.0;
                    for (k = 0; k < N; k++) {
                        sum += A[i][k] * B[k][j];
                    }
                    C[i][j] = sum;
                }
            }

            end_time = omp_get_wtime();

            double exec_time = end_time - start_time;
            printf("%-12d %-7d %.6f\n", N, t, exec_time);

            fprintf(fp, "%d,%d,%f\n", N, t, exec_time);
        }

        for (i = 0; i < N; i++) {
            free(A[i]);
            free(B[i]);
            free(C[i]);
        }
        free(A);
        free(B);
        free(C);
    }

    fclose(fp);
    return 0;
}
