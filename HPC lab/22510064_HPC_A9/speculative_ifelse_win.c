#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define SIZE 10000000

int main(int argc, char **argv) {
    int num_threads = 4;
    if (argc >= 2) num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    double *x = (double*)malloc(SIZE * sizeof(double));
    double *out_seq = (double*)malloc(SIZE * sizeof(double));
    double *out_spec = (double*)malloc(SIZE * sizeof(double));

    for (int i = 0; i < SIZE; i++) {
        x[i] = (i % 2 == 0) ? (double)(i % 1000 + 1) : (double)(-1.0 * (i % 1000 + 1));
    }

    double t0 = omp_get_wtime();
    long long seq_ops = 0;
    for (int i = 0; i < SIZE; i++) {
        double xi = x[i];
        if (xi > 0) { out_seq[i] = sqrt(xi); seq_ops++; }
        else { out_seq[i] = log(fabs(xi)); seq_ops++; }
    }
    double t_seq = omp_get_wtime() - t0;

    double t1 = omp_get_wtime();
    long long spec_ops = 0;
    long long wasted_ops = 0;

    #pragma omp parallel for reduction(+:spec_ops,wasted_ops)
    for (int i = 0; i < SIZE; i++) {
        double xi = x[i];
        double a = sqrt(fabs(xi));  
        double b = log(fabs(xi));  
        spec_ops += 2;

        if (xi > 0) { out_spec[i] = a; wasted_ops += 1; }
        else { out_spec[i] = b; wasted_ops += 1; }
    }
    double t_spec = omp_get_wtime() - t1;

    double wasted_percent = 100.0 * ((double)wasted_ops / (double)spec_ops);

    printf("speculation,SIZE=%d,threads=%d,t_seq=%f,t_spec=%f,spec_ops=%lld,wasted_ops=%lld,wasted_pct=%f\n",
           SIZE, num_threads, t_seq, t_spec, spec_ops, wasted_ops, wasted_percent);
    fflush(stdout); 

    free(x); free(out_seq); free(out_spec);
    return 0;
}
