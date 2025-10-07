#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int N;


int is_safe(int *cols, int row, int c){
for(int i=0;i<row;i++){
if(cols[i]==c) return 0;
if(abs(cols[i]-c)==(row-i)) return 0;
}
return 1;
}


long long solve_from(int *cols, int row){
if(row==N) return 1; 
long long count=0;
for(int c=0;c<N;c++){
if(is_safe(cols,row,c)){
cols[row]=c;
count += solve_from(cols,row+1);
}
}
return count;
}


int main(int argc,char **argv){
if(argc<3){
printf("Usage: %s N num_threads\n",argv[0]);
return 1;
}
N = atoi(argv[1]);
int num_threads = atoi(argv[2]);
omp_set_num_threads(num_threads);


long long total=0;
double t0 = omp_get_wtime();


#pragma omp parallel
{
int tid = omp_get_thread_num();
int threads = omp_get_num_threads();
int per = (N + threads - 1)/threads;
int start = tid * per;
int end = start + per;
if(end > N) end = N;
long long local_count = 0;
int *cols = (int*)malloc(N * sizeof(int));
for(int c=start;c<end;c++){

    for(int i=0;i<N;i++) cols[i] = -1;
cols[0]=c;
local_count += solve_from(cols,1);
}
free(cols);
#pragma omp atomic
total += local_count;
}


double t1 = omp_get_wtime() - t0;
printf("nqueens,N=%d,threads=%d,solutions=%lld,time=%f\n",N,num_threads,total,t1);
return 0;
}