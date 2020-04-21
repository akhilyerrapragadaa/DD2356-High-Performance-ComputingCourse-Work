#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define N 1000000

int main()
{
    int x[N];
    clock_t begin = clock();
    double time_spent = 0.0;
    
    printf("begin %ld \n", begin);
    srand(time(0)); // seed
    

   for(size_t i=0; i < N;i++){
     // Generate random number between 0 and 1
     x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
   //  printf("%d \n", x[i]);
   }
   

int MAX_THREADS = omp_get_num_threads();
int maxloc[MAX_THREADS], mloc;
double maxval[MAX_THREADS], mval;
#pragma omp parallel shared(maxval,maxloc)
{

int id = omp_get_thread_num();
maxval[id] = -1.0e30;

#pragma omp for
  for (size_t j=0; j < N; j++){
       if (x[j] > maxval[id]) {
            maxval[id] = x[j]; 
            maxloc[id] = j;
            }
  }
  
}

mloc = maxloc[0];
mval = maxval[0];
for(size_t l=1; l< MAX_THREADS;l++){
    if(maxval[l] > mval){
        mval = maxval[l];
        mloc = maxloc[l];
    }
}


  clock_t end = clock();
  time_spent = (double)(end - begin)/ CLOCKS_PER_SEC; 

printf("Value is %f \n", time_spent);
    return 0;
}

