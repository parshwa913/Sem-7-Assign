#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
    int tests;
    printf("Enter number of test runs: ");
    scanf("%d", &tests);
    int threads[tests], sizes[tests];
    double times[tests];
    for (int t = 0; t < tests; t++) {
        printf("Test %d - Enter number of threads and array size: ", t + 1);
        scanf("%d %d", &threads[t], &sizes[t]);
        omp_set_num_threads(threads[t]);
        float *a = (float *)malloc(sizes[t] * sizeof(float));
        float *b = (float *)malloc(sizes[t] * sizeof(float));
        float scalar = 5.0;
        for (int i = 0; i < sizes[t]; i++) {
            a[i] = i * 1.0;
        }
        double start = omp_get_wtime();

        #pragma omp parallel for
        for (int i = 0; i < sizes[t]; i++) {
            b[i] = a[i] + scalar;
        }
        double end = omp_get_wtime();
        times[t] = end - start;
        free(a);
        free(b);
    }
    printf("\n--- Vector Scalar Addition Results ---\n");
    printf("Test\tThreads\tSize\t\tTime (sec)\n");
    for (int i = 0; i < tests; i++) {
        printf("%d\t%d\t%d\t\t%.6f\n", i + 1, threads[i], sizes[i], times[i]);
    }
    return 0;
}
