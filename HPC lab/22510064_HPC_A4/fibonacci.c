#include <stdio.h>
#include <omp.h>

int main() {
    int n, i;
    printf("Enter number of Fibonacci terms: ");
    scanf("%d", &n);

    if (n < 1) {
        printf("Number of terms must be positive.\n");
        return 0;
    }

    long long fib[n];
    fib[0] = 0;
    if (n > 1) fib[1] = 1;

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (i = 2; i < n; i++) {
                #pragma omp critical
                {
                    fib[i] = fib[i - 1] + fib[i - 2];
                }
            }
        }
    }

    printf("Fibonacci Series: ");
    for (i = 0; i < n; i++)
        printf("%lld ", fib[i]);
    printf("\n");

    return 0;
}
