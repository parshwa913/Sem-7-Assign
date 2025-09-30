#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define VEC_SIZE 10000000

int main(int argc,char**argv){
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int base=VEC_SIZE/size;
    int extra=VEC_SIZE%size;
    int local_n=(rank<extra)?base+1:base;
    int start=(rank<extra)?rank*local_n:rank*local_n+extra;

    double *a=(double*)malloc(local_n*sizeof(double));
    double *b=(double*)malloc(local_n*sizeof(double));
    for(int i=0;i<local_n;i++){
        long idx=start+i;
        a[i]=1.0;
        b[i]=2.0 + (double)(idx%5)/5.0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t0=MPI_Wtime();

    double local_dot=0.0;
    for(int i=0;i<local_n;i++)
        local_dot+=a[i]*b[i];

    double global_dot=0.0;
    MPI_Reduce(&local_dot,&global_dot,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    double t1=MPI_Wtime()-t0;
    if(rank==0)
        printf("dot,VEC=%d,procs=%d,time=%f,dot=%f\n",
               VEC_SIZE,size,t1,global_dot);

    free(a);
    free(b);
    MPI_Finalize();
    return 0;
}
