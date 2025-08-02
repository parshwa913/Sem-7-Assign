#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter size of vectors: ");
    scanf("%d", &n);

    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));
    printf("Enter elements of first vector:\n");
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    printf("Enter elements of second vector:\n");
    for (int i = 0; i < n; i++) scanf("%d", &b[i]);

    int dot = 0;
    #pragma omp parallel for reduction(+:dot) schedule(static)
    for (int i = 0; i < n; i++) {
        dot += a[i] * b[i];
    }

    printf("Parallel Dot Product: %d\n", dot);
    free(a);
    free(b);
    return 0;
}