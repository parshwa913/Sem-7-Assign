#include <stdio.h>
#include <omp.h>
int main() {
    int tests;
    printf("Enter number of test runs: ");
    scanf("%d", &tests);
    int threads[tests];
    long steps[tests];
    double times[tests], results[tests];
    for (int t = 0; t < tests; t++) {
        printf("Test %d - Enter number of threads and steps: ", t + 1);
        scanf("%d %ld", &threads[t], &steps[t]);
        omp_set_num_threads(threads[t]);
        double sum = 0.0, x, pi;
        double step = 1.0 / (double)steps[t];
        double start = omp_get_wtime();
        #pragma omp parallel for private(x) reduction(+:sum)
        for (long i = 0; i < steps[t]; i++) {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        pi = step * sum;
        double end = omp_get_wtime();
        results[t] = pi;
        times[t] = end - start;
    }
    printf("\n--- Pi Calculation Results ---\n");
    printf("Test\tThreads\tSteps\t\tTime (sec)\tPi Estimate\n");
    for (int i = 0; i < tests; i++) {
        printf("%d\t%d\t%ld\t\t%.6f\t%.12f\n", i + 1, threads[i], steps[i], times[i], results[i]);
    }
    return 0;
}
