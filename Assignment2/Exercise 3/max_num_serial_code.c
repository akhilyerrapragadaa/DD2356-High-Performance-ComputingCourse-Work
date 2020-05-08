#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define N 1000000

int main()
{
    int x[N];
    double time_spent = 0.0;
    double maxval = 0.0; 
    int maxloc = 0;
    clock_t begin = clock();
    srand(time(0)); // seed
    
   for(size_t i=0; i < N;i++){
     // Generate random number between 0 and 1
     x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
   //  printf("%d \n", x[i]);
   }
   

  for (size_t j=0; j < N; j++){
       if (x[j] > maxval) {
            maxval = x[j]; 
            maxloc = j;
            }
  }
  
  clock_t end = clock();
  time_spent = (double)(end - begin)/ CLOCKS_PER_SEC; 

printf("Value is %f \n", time_spent);
    return 0;
}

