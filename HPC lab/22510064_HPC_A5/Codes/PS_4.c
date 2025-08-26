#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void run_prefix_sum(int N, int num_threads, FILE *csv_file) {
    int i;
    double *arr, *prefix;
    double start_time, end_time;

    arr = (double *)malloc(N * sizeof(double));
    prefix = (double *)malloc(N * sizeof(double));

    for (i = 0; i < N; i++) {
        arr[i] = 1;
    }

    omp_set_num_threads(num_threads);

    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        int chunk_size = N / nthreads;
        int start = id * chunk_size;
        int end = (id == nthreads - 1) ? N : start + chunk_size;

        prefix[start] = arr[start];
        for (i = start + 1; i < end; i++) {
            prefix[i] = prefix[i - 1] + arr[i];
        }

        double block_sum = prefix[end - 1];

        #pragma omp barrier
        double offset = 0.0;
        for (i = 0; i < id; i++) {
            int prev_end = (i + 1) * chunk_size;
            offset += prefix[prev_end - 1];
        }

        if (id != 0) {
            for (i = start; i < end; i++) {
                prefix[i] += offset;
            }
        }
    }

    end_time = omp_get_wtime();

    double exec_time = end_time - start_time;

    printf("%-12d %-7d %f\n", N, num_threads, exec_time);

    fprintf(csv_file, "%d,%d,%f\n", N, num_threads, exec_time);

    free(arr);
    free(prefix);
}

int main() {
    int data_sizes[] = {10000, 50000, 100000, 500000, 1000000};
    int num_sizes = sizeof(data_sizes) / sizeof(data_sizes[0]);

    int thread_counts[] = {1, 2, 4, 8};
    int num_threads = sizeof(thread_counts) / sizeof(thread_counts[0]);

    FILE *csv_file = fopen("PS_4.csv", "w");
    if (!csv_file) {
        printf("Error opening CSV file!\n");
        return 1;
    }

    fprintf(csv_file, "DataSize,Threads,Time(s)\n");

    printf("%-12s %-7s %s\n", "DataSize", "Threads", "Time(s)");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < num_sizes; i++) {
        for (int j = 0; j < num_threads; j++) {
            run_prefix_sum(data_sizes[i], thread_counts[j], csv_file);
        }
    }

    fclose(csv_file);
    return 0;
}
