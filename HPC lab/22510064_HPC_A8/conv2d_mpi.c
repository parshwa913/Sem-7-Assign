#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define NX 1024
#define NY 1024
#define K 3

int main(int argc,char**argv){
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int rows_per_proc=NX/size;
    int extra=NX%size;
    int start_row,local_nrows;
    if(rank<extra){
        local_nrows=rows_per_proc+1;
        start_row=rank*local_nrows;
    }else{
        local_nrows=rows_per_proc;
        start_row=rank*local_nrows+extra;
    }
    int alloc_rows=local_nrows+2;

    double *local_in=(double*)malloc((alloc_rows)*NY*sizeof(double));
    double *local_out=(double*)malloc((local_nrows)*NY*sizeof(double));
    double kernel[K*K]={0.0625,0.125,0.0625,
                        0.125,0.25,0.125,
                        0.0625,0.125,0.0625};

    for(int i=0;i<alloc_rows;i++)
        for(int j=0;j<NY;j++){
            int global_i=start_row+i-1;
            if(global_i<0||global_i>=NX)
                local_in[i*NY+j]=0.0;
            else
                local_in[i*NY+j]= (double)((global_i*NY+j)%100)/100.0 + 1.0;
        }

    MPI_Barrier(MPI_COMM_WORLD);
    double t0=MPI_Wtime();

    for(int step=0;step<1;step++){
        if(size>1){
            if(rank>0)
                MPI_Sendrecv(&local_in[1*NY],NY,MPI_DOUBLE,rank-1,0,
                             &local_in[0],NY,MPI_DOUBLE,rank-1,1,
                             MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            if(rank<size-1)
                MPI_Sendrecv(&local_in[(local_nrows)*NY],NY,MPI_DOUBLE,rank+1,1,
                             &local_in[(local_nrows+1)*NY],NY,MPI_DOUBLE,rank+1,0,
                             MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

        double comp0=MPI_Wtime();
        for(int i=1;i<=local_nrows;i++){
            for(int j=0;j<NY;j++){
                double sum=0.0;
                for(int ki=0;ki<K;ki++)
                    for(int kj=0;kj<K;kj++){
                        int ii=i+(ki-1);
                        int jj=j+(kj-1);
                        double v=0.0;
                        if(jj>=0 && jj<NY && ii>=0 && ii<alloc_rows)
                            v=local_in[ii*NY+jj];
                        sum+=kernel[ki*K+kj]*v;
                    }
                if(i-1<local_nrows)
                    local_out[(i-1)*NY+j]=sum;
            }
        }
        double comp1=MPI_Wtime()-comp0;
        double t1=MPI_Wtime()-t0;
        if(rank==0)
            printf("conv,NX=%d,NY=%d,procs=%d,time_total=%f,time_comp=%f\n",
                   NX,NY,size,t1,comp1);
    }

    free(local_in);
    free(local_out);
    MPI_Finalize();
    return 0;
}
