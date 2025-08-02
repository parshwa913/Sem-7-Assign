#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
    int size, chunk, threads;
    printf("Enter vector size: ");
    scanf("%d", &size);
    printf("Enter chunk size: ");
    scanf("%d", &chunk);
    printf("Enter number of threads: ");
    scanf("%d", &threads);
    int *A = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) A[i] = rand() % 100;
    omp_set_num_threads(threads);
    omp_set_schedule(omp_sched_static, chunk);
    double start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp for schedule(runtime) nowait
        for (int i = 0; i < size; i++)
            A[i] += 5;
    }
    double end = omp_get_wtime();
    printf("Schedule: Static, Chunk: %d, Threads: %d, Time: %f sec\n", chunk, threads, end - start);
    omp_set_schedule(omp_sched_dynamic, chunk);
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp for schedule(runtime) nowait
        for (int i = 0; i < size; i++)
            A[i] += 5;
    }
    end = omp_get_wtime();
    printf("Schedule: Dynamic, Chunk: %d, Threads: %d, Time: %f sec\n", chunk, threads, end - start);
    free(A);
    return 0;
}