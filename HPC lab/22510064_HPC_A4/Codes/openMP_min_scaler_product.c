#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// sort asendingly
int compare_asc(const void* a, const void* b) {
    double val1 = *(const double*)a;
    double val2 = *(const double*)b;
    if (val1 < val2) return -1;
    if (val1 > val2) return 1;
    return 0;
}

// sort descendingly
int compare_desc(const void* a, const void* b) {
    double val1 = *(const double*)a;
    double val2 = *(const double*)b;
    if (val1 > val2) return -1;
    if (val1 < val2) return 1;
    return 0;
}

int main() {
    long n = 50000000; 
    // as per cpu of my intel i5 12th gen
    int num_threads = 12; 

    omp_set_num_threads(num_threads);

    // memory
    double* vec_a = (double*)malloc(n * sizeof(double));
    double* vec_b = (double*)malloc(n * sizeof(double));
    if (vec_a == NULL || vec_b == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // initialization
    srand(time(NULL));
    for (long i = 0; i < n; i++) {
        vec_a[i] = (double)rand() / RAND_MAX * 100.0;
        vec_b[i] = (double)rand() / RAND_MAX * 100.0;
    }

    printf("Minimum Scalar Product Calculation\n");
    printf("Vector Size: %ld\n", n);
    printf("Using %d threads for the parallel part.\n\n", num_threads);

    // sequential sorting 
    double start_sort = omp_get_wtime();
    qsort(vec_a, n, sizeof(double), compare_asc);
    qsort(vec_b, n, sizeof(double), compare_desc);
    double end_sort = omp_get_wtime();
    double time_sort = end_sort - start_sort;
    printf("Phase 1: Sorting (Serial) took %.4f seconds.\n", time_sort);

    // parallel dot product
    double min_product = 0.0;
    double start_dot = omp_get_wtime();

    
    #pragma omp parallel for reduction(+:min_product)
    for (long i = 0; i < n; i++) {
        min_product += vec_a[i] * vec_b[i];
    }
    double end_dot = omp_get_wtime();
    double time_dot = end_dot - start_dot;
    printf("Phase 2: Dot Product (Parallel) took %.4f seconds.\n\n", time_dot);

    
    
    printf("Minimum Scalar Product: %f\n", min_product);
    printf("Total execution time:   %.4f seconds\n", time_sort + time_dot);


    free(vec_a);
    free(vec_b);

    return 0;
}