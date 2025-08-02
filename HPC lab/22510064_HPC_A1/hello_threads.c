#include <stdio.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter number of threads: ");
    scanf("%d", &n);

    printf("\nSequential Hello:\n");
    for (int i = 0; i < n; i++) {
        printf("Hello, World. %d\n", i);
    }

    printf("\nParallel Hello:\n");
    omp_set_num_threads(9); // Set number of threads to 9
    #pragma omp parallel
    {
        printf("Hello, World. %d\n", omp_get_thread_num());
    }

    return 0;
}