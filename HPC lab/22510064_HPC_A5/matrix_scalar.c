#include <stdio.h>
#include <omp.h>

int main() {
    int r, c, scalar;
    printf("Enter rows and cols of matrix: ");
    scanf("%d %d", &r, &c);
    printf("Enter scalar value: ");
    scanf("%d", &scalar);

    int A[r][c];

    printf("Enter elements of matrix:\n");
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            scanf("%d", &A[i][j]);

    #pragma omp parallel for
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            A[i][j] *= scalar;

    printf("Resultant Matrix:\n");
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }
    return 0;
}
