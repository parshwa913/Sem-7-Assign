#include <stdio.h>
#include <omp.h>

#define SIZE 5

int main() {
    int buffer[SIZE];
    int count = 0; // items in buffer
    int i;

    #pragma omp parallel num_threads(2) shared(buffer, count) private(i)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) {
            // Producer
            for (i = 0; i < SIZE; i++) {
                #pragma omp critical
                {
                    buffer[count] = i * 10;
                    printf("Producer produced: %d\n", buffer[count]);
                    count++;
                }
                #pragma omp barrier
            }
        } 
        else {
            // Consumer
            for (i = 0; i < SIZE; i++) {
                #pragma omp barrier
                #pragma omp critical
                {
                    printf("Consumer consumed: %d\n", buffer[count - 1]);
                    count--;
                }
            }
        }
    }
    return 0;
}
