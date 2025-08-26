#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void run_matrix_vector_mult(int N, int M, int threads, FILE *fp) {
    int i, j;
    double **A, *x, *y;
    double start_time, end_time;

    A = (double **)malloc(N * sizeof(double *));
    for (i = 0; i < N; i++) {
        A[i] = (double *)malloc(M * sizeof(double));
    }
    x = (double *)malloc(M * sizeof(double));
    y = (double *)malloc(N * sizeof(double));

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            A[i][j] = rand() % 100;
        }
    }
    for (j = 0; j < M; j++) {
        x[j] = rand() % 100;
    }

    omp_set_num_threads(threads);
    start_time = omp_get_wtime();

    #pragma omp parallel for private(i, j) shared(A, x, y)
    for (i = 0; i < N; i++) {
        double sum = 0.0;
        for (j = 0; j < M; j++) {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }

    end_time = omp_get_wtime();
    double exec_time = end_time - start_time;

    printf("%-12d %-7d %f\n", N*M, threads, exec_time);

    fprintf(fp, "%d,%d,%f\n", N*M, threads, exec_time);

    for (i = 0; i < N; i++) free(A[i]);
    free(A);
    free(x);
    free(y);
}

int main() {
    int sizes[] = {200, 500, 1000, 1500};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int thread_counts[] = {1, 2, 4, 8};
    int num_threads = sizeof(thread_counts) / sizeof(thread_counts[0]);

    FILE *fp = fopen("PS_3.csv", "w");
    if (!fp) {
        printf("Error opening file!\n");
        return 1;
    }

    // CSV Header
    fprintf(fp, "DataSize,Threads,Time(s)\n");

    // Print Header
    printf("%-12s %-7s %s\n", "DataSize", "Threads", "Time(s)");
    printf("--------------------------------------------------\n");

    // Run for different sizes and thread counts
    for (int i = 0; i < num_sizes; i++) {
        for (int j = 0; j < num_threads; j++) {
            run_matrix_vector_mult(sizes[i], sizes[i], thread_counts[j], fp);
        }
    }

    fclose(fp);
    return 0;
}
