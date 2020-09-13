#include <stdio.h>
 
int main (void)
{
  printf ("serial region 1\n");
 
#pragma omp parallel
  {
    printf ("parallel region\n");
  }
 
  printf ("serial region 2\n");
 
  return 0;
}

// Без опции /openmp:experimental
// serial region 1
// parallel region
// serial region 2

// С опцией /openmp:experimental
// serial region 1
// parallel region
// parallel region
// parallel region
// parallel region
// serial region 2