#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
    int size, threads;
    printf("Enter matrix size (NxN): ");
    scanf("%d", &size);
    printf("Enter number of threads: ");
    scanf("%d", &threads);
    omp_set_num_threads(threads);
    int **A = malloc(size * sizeof(int*));
    int **B = malloc(size * sizeof(int*));
    int **C = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        A[i] = malloc(size * sizeof(int));
        B[i] = malloc(size * sizeof(int));
        C[i] = malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
    double start = omp_get_wtime();
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C[i][j] = A[i][j] + B[i][j];
    double end = omp_get_wtime();
    printf("Matrix Size: %d, Threads: %d, Time: %f sec\n", size, threads, end - start);
    for (int i = 0; i < size; i++) {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);
    return 0;
}