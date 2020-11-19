#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x86\\msmpi.lib")

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define N 10
#define NNN N*N*N

void printfArray(double *d, int n, double t) {
    printf("%.8f\n", t);
    for (int i = 0; i < n; i++) {
        printf("%.7f ", d[i]);
    }
    printf("\n");
}

void f(int rank, int start_n, int end_n, double t, double *x, double *dxdt, int n) {
    if (rank == 0) {
        int i = 0;
        dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1+n]) * 0.5 * N
                - (x[i+2] - 2 * x[i+1] + 2 * x[i-1+n] - x[i-2+n]) * 0.5 * NNN;
        i = 1;
        dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1]) * 0.5 * N
                - (x[i+2] - 2 * x[i+1] + 2 * x[i-1] - x[i-2+n]) * 0.5 * NNN;
        for (i = 2; i < end_n; i++)
        {
            dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1]) * 0.5 * N
                    - (x[i+2] - 2 * x[i+1] + 2 * x[i-1] - x[i-2]) * 0.5 * NNN;
        }
    }
    if (rank == 1) {
        int i = start_n;
        for (i = start_n; i < n-2; i++)
        {
            dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1]) * 0.5 * N
                    - (x[i+2] - 2 * x[i+1] + 2 * x[i-1] - x[i-2]) * 0.5 * NNN;
        }
        i = n-2;
        dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1]) * 0.5 * N
                - (x[i+2-n] - 2 * x[i+1] + 2 * x[i-1] - x[i-2]) * 0.5 * NNN;
        i = n-1;
        dxdt[i] = - 6 * x[i] * (x[i+1-n] - x[i-1]) * 0.5 * N
                - (x[i+2-n] - 2 * x[i+1-n] + 2 * x[i-1] - x[i-2]) * 0.5 * NNN;
    }
}

int rk4(int rank, int start_n, int end_n, int n, double t, double *x, double h, double finish) {

    if (h <= 0 || (finish - t) <= 0) {
        return -1;
    }

    double *k1, *k2, *k3, *k4;
    double *temp;
    k1 = (double*) malloc(n * sizeof (double));
    k2 = (double*) malloc(n * sizeof (double));
    k3 = (double*) malloc(n * sizeof (double));
    k4 = (double*) malloc(n * sizeof (double));
    temp = (double*) malloc(n * sizeof (double));

    while (true) {
        if (t > finish) {
            break;
        }               
        
        if (rank == 0) {
            MPI_Status status;
            MPI_Recv(&(x[start_n]), n-start_n, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&(x[start_n-2]), 1, MPI_FLOAT, 1, 1, MPI_COMM_WORLD);
            MPI_Send(&(x[start_n-1]), 1, MPI_FLOAT, 1, 2, MPI_COMM_WORLD);	
            printfArray(x, n, t);
        } 
        if (rank == 1) {
            MPI_Status status;
            MPI_Send(&(x[start_n]), n-start_n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&(x[start_n-2]), 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&(x[start_n-1]), 1, MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status);
        }

        //k1
        f(rank, start_n, end_n, t, x, k1, n);
        //k2
        for (int i = start_n; i < end_n; i++) {
            temp[i] = x[i] + 0.5 * h * k1[i];
        }
        f(rank, start_n, end_n, t + 0.5 * h, temp, k2, n);
        //k3
        for (int i = start_n; i < end_n; i++) {
            temp[i] = x[i] + 0.5 * h * k2[i];
        }
        f(rank, start_n, end_n, t + 0.5 * h, temp, k3, n);
        //k4
        for (int i = start_n; i < end_n; i++) {
            temp[i] = x[i] + h * k3[i];
        }
        f(rank, start_n, end_n, t + h, temp, k4, n);
        //res
        for (int i = start_n; i < end_n; i++) {
            x[i] += h * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6.0;
        }

        t += h; 
    }

    free(k1);
    free(k2);
    free(k3);
    free(k4);
    free(temp);
    return 0;
}

int main(int argc, char * argv[]) {    
    system("chcp 65001"); // utf-8
    int rank = 0,  size = 2;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        freopen("output.txt", "w", stdout);
    }
    int n = 50*N; // amount x
    int center_n = int (n / 2);
    int start_n = 0, end_n = n;
    switch (rank)
    {
    case 0:
        start_n = 0;
        end_n = center_n;
        break;
    case 1:
        start_n = center_n;
        end_n = n;
        break;
    default:
        freopen("CON", "w", stdout);
        printf("\nРассчитано на 2 процесса. Запущенный процесс %d закрыт.\n", rank);
        MPI_Finalize();
        return 0;
    }
    double h = 0.001; // step t (time)

    double *x = (double*) malloc(n * sizeof (double));
    double from = 0.0, to = 10.0; // t (time)

    double k = 1.0;
    for (int i = start_n; i < end_n; i++)
    {
        double xx = i*0.1;
        x[i] = 2.0 * k * k / (cosh(k * (xx - 25)) * cosh(k * (xx - 25)));
    }

    rk4(rank, start_n, end_n, n, from, x, h, to);
    
    free(x);
    MPI_Finalize();
}
