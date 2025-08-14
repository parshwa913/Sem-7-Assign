#include <stdio.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n], prefix[n];
    printf("Enter elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    prefix[0] = arr[0];
    for (int i = 1; i < n; i++) {
        prefix[i] = prefix[i - 1] + arr[i];
    }

    printf("Prefix Sum Array:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", prefix[i]);
    printf("\n");

    return 0;
}
