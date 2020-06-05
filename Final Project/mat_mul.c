#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "mpi.h"
#include "time.h"
#include "unistd.h"
#define N 3

int main(int argc, char *argv[]){

MPI_Init(&argc, &argv);
//Input
int a, b, c = 0;
int Areceived, Breceived;
srand(time(NULL));

int periods[2] = {1,1};
int dims[2] = {N,N}, wsize, source, destination, rank, size, dummysize, columnsize;
int coord[2];
int newRank, myRow, columnrank;

MPI_Comm cartcomm, newcomm, columncomm;
MPI_Comm_size(MPI_COMM_WORLD, &wsize);

MPI_Dims_create(wsize, 2, dims);
MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cartcomm);
MPI_Comm_rank(cartcomm, &rank);
MPI_Cart_coords(cartcomm, rank, 2, coord);
//printf("Rank:%d with Coords: %d%d\n", rank, coord[0], coord[1]);


myRow = (int)(rank/N);
MPI_Comm_split(cartcomm, myRow, rank, &newcomm);
MPI_Comm_split(cartcomm, coord[1], rank, &columncomm);
MPI_Comm_rank(newcomm, &newRank);
MPI_Comm_size(newcomm, &size);

MPI_Comm_rank(columncomm, &columnrank);
MPI_Comm_size(columncomm, &columnsize);
//printf("Column rank is:%d coords:%d%d\n", columnrank, coord[0], coord[1]);
//printf("Size is:%d\n", columnsize);

for(int R = 0; R < N ; R++){
    for(int C = 0; C < N ; C++){
        if(R == coord[0] && C == coord[1]){
            a = rand() % 549 + R + C;
            b = rand() % 837 + R + C;
        }
    }
}

for(int D = 0; D < N; D++){
    printf("Iter : %d ",D);
   //Row Broadcast
    for(int R = 0; R < N; R++){
       sleep(2);
       if((R+D) < N){
       
       if(R == coord[0] && R + D == coord[1]){
              Areceived = a;
              printf("Coords: %d%d\n",coord[0], coord[1]);
              printf("************************Row :%d Buf value sent is: %d and Rank: %d\n", R, a, newRank);
              MPI_Bcast(&a, 1, MPI_INT, R + D, newcomm);

       }
       else{
       if(R == coord[0]){
             MPI_Bcast(&Areceived, 1, MPI_INT, R + D, newcomm);
             printf("Coords: %d%d\n",coord[0], coord[1]);
             printf("Row :%d Buf value received is: %d and Rank: %d\n", R, Areceived, newRank);
       }
       }
          
       }
       else{
       if(R == coord[0] &&  R + D - N  == coord[1]){
              Areceived = a;
              printf("Coords: %d%d\n",coord[0], coord[1]);
              printf("************************Row :%d Buf value sent is: %d and Rank: %d\n", R, a, newRank);
              MPI_Bcast(&a, 1, MPI_INT, R + D - N, newcomm);
       }
       else{
       if(R == coord[0]){
             MPI_Bcast(&Areceived, 1, MPI_INT, R + D - N, newcomm);
             printf("Coords: %d%d\n",coord[0], coord[1]);
             printf("Row :%d Buf value received is: %d and Rank: %d\n", R, Areceived, newRank);
       }
       }

       }   
    }
    
    
    
//Cloumn broadcast
  int I;
if(D != 0){

for(int C = 0; C < N; C++){
  
    for(I = N; I > 0; I--){

if(I != 1){
    if(coord[0] == I-1 && C == coord[1]){   
            printf("************************Column :%d Buf value sent is: %d and Rank: %d\n", C, b, columnrank);
            MPI_Bcast(&b, 1, MPI_INT, I-1, columncomm);
    }
    else{  
        if(coord[0] == I-2 && coord[1] == C){
        //sleep(5);
           MPI_Bcast(&Breceived, 1, MPI_INT, I-1, columncomm);   
           printf("Column :%d Buf value received is: %d and Rank: %d\n", C, Breceived, columnrank);
        }

    }
 }
 else{
       
    if(coord[0] == I-1 && C == coord[1]){   
            printf("************************Column :%d Buf value sent is: %d and Rank: %d\n", C, b, columnrank);
            MPI_Bcast(&b, 1, MPI_INT, I-1, columncomm);
    }
    else{
        if(coord[0] == N-1 && coord[1] == C){
        //sleep(5);
           MPI_Bcast(&Breceived, 1, MPI_INT, I-1, columncomm);   
           printf("Column :%d Buf value received is: %d and Rank: %d\n", C, Breceived, columnrank);
        }
    }
 
}

if(I == 1){
   b = Breceived; 
}


}
    } 
}
else{
    Breceived = b;
}

sleep(2);
//Matrix Multiply
printf("Iteration :%d ", D+1);
for(int R = 0; R < N ; R++){
    for(int C = 0; C < N; C++){
        if(R == coord[0] && C == coord[1]){         
            c = c + (Areceived * Breceived);
            printf("################Multiplied values A: %d and B: %d from :%d%d is %d\n", Areceived, Breceived,coord[0], coord[1], c);
        }
    }
}


}

MPI_Finalize();
return 0;
}

