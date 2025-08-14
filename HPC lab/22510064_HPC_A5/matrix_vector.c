#include <stdio.h>
#include <omp.h>

int main() {
    int r, c;
    printf("Enter rows and cols of matrix: ");
    scanf("%d %d", &r, &c);

    int A[r][c], V[c], R[r];

    printf("Enter elements of matrix:\n");
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of vector:\n");
    for (int i = 0; i < c; i++)
        scanf("%d", &V[i]);

    #pragma omp parallel for
    for (int i = 0; i < r; i++) {
        R[i] = 0;
        for (int j = 0; j < c; j++)
            R[i] += A[i][j] * V[j];
    }

    printf("Resultant Vector:\n");
    for (int i = 0; i < r; i++)
        printf("%d ", R[i]);
    printf("\n");

    return 0;
}
