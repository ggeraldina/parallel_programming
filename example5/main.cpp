#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x86\\msmpi.lib")

#include "mpi.h"
#include <stdio.h>
int main(int argc, char **argv)
{
   int rank;
   float a, b;
   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   a = 0.0;
   b = 0.0;
   if(rank == 0)
   {
      b = 3.14159;
	  MPI_Sendrecv(&b, 1, MPI_FLOAT, 1, 5, &a, 1, MPI_FLOAT, 1, 5, MPI_COMM_WORLD, &status);
   }
   if(rank == 1)
   {
      a = 2.71828;
	  MPI_Sendrecv(&a, 1, MPI_FLOAT, 0, 5, &b, 1, MPI_FLOAT, 0, 5, MPI_COMM_WORLD, &status);
   }
   printf("process %d a = %f, b = %f\n", rank, a, b);
   MPI_Finalize();
}

// запуск 
// mpiexec -n 4 Debug\main.exe 


// process 0 a = 2.718280, b = 3.141590
// process 3 a = 0.000000, b = 0.000000
// process 1 a = 2.718280, b = 3.141590
// process 2 a = 0.000000, b = 0.000000