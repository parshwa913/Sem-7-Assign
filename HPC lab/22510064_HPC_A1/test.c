//the below code prints the message "Hello, world." 
//for the number of threads specified by the number of actual threads available in the system
//my system has 8 cores,12 threads 
//default answer will likely be 12 times message printed
#include <stdio.h>
#include <omp.h>

int main(void)
{
    #pragma omp parallel
    printf("Hello, world.\n");
    return 0;
}

// #include <stdio.h>
// #include <omp.h>

// int main(void)
// {
//     omp_set_num_threads(4); // the code here sets the number of threads to 4

    // #pragma omp parallel
    // printf("Hello, world. %d\n", omp_get_thread_num());

    // return 0;
// }

