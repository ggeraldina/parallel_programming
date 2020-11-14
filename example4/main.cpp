#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 10
#define NNN N*N*N

void printfArray(double *d, int n, double t) {
    printf("%.8f\n", t);
    for (int i = 0; i < n; i++) {
        printf("%.7f ", d[i]);
    }
    printf("\n");
}

void f(double t, double *x, double *dxdt, int n) {
    int i = 0;
    dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1+n]) * 0.5 * N
              - (x[i+2] - 2 * x[i+1] + 2 * x[i-1+n] - x[i-2+n]) * 0.5 * NNN;
    i = 1;
    dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1]) * 0.5 * N
              - (x[i+2] - 2 * x[i+1] + 2 * x[i-1] - x[i-2+n]) * 0.5 * NNN;
#pragma omp parallel for
    for (i = 2; i < n-2; i++)
    {
        dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1]) * 0.5 * N
                  - (x[i+2] - 2 * x[i+1] + 2 * x[i-1] - x[i-2]) * 0.5 * NNN;
        
        printf(
            "\n!!!fun %d Закончил работу поток №%d из %d потоков\n", 
            i,
            omp_get_thread_num(), 
            omp_get_num_threads()
        );
    }
    i = n-2;
    dxdt[i] = - 6 * x[i] * (x[i+1] - x[i-1]) * 0.5 * N
              - (x[i+2-n] - 2 * x[i+1] + 2 * x[i-1] - x[i-2]) * 0.5 * NNN;
    i = n-1;
    dxdt[i] = - 6 * x[i] * (x[i+1-n] - x[i-1]) * 0.5 * N
              - (x[i+2-n] - 2 * x[i+1-n] + 2 * x[i-1] - x[i-2]) * 0.5 * NNN;
}

int rk4(int n, double t, double *x, double h, double finish) {

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
        
        printfArray(x, n, t);

        //k1
        f(t, x, k1, n);
        //k2
#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + 0.5 * h * k1[i];
            printf(
                "\nk2 %d Закончил работу поток №%d из %d потоков\n", 
                i,
                omp_get_thread_num(), 
                omp_get_num_threads()
            );
        }
        f(t + 0.5 * h, temp, k2, n);
        //k3
#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + 0.5 * h * k2[i];
            printf(
                "\nk3 %d Закончил работу поток №%d из %d потоков\n", 
                i,
                omp_get_thread_num(), 
                omp_get_num_threads()
            );
        }
        f(t + 0.5 * h, temp, k3, n);
        //k4
#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + h * k3[i];
            printf(
                "\nk4 %d Закончил работу поток №%d из %d потоков\n", 
                i,
                omp_get_thread_num(), 
                omp_get_num_threads()
            );
        }
        f(t + h, temp, k4, n);
        //res
#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            x[i] += h * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6.0;
            printf(
                "\nx %d Закончил работу поток №%d из %d потоков\n", 
                i,
                omp_get_thread_num(), 
                omp_get_num_threads()
            );
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
    freopen("output.txt", "w", stdout);
    int n = 50*N; // amount x
    double h = 0.001; // step t (time)
    double *x = (double*) malloc(n * sizeof (double));
    double from = 0.0, to = 10.0; // t (time)

    double start_time = 0., end_time = 0.;
    omp_set_num_threads(4);
    start_time = omp_get_wtime();

    double k = 1.0;
#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        double xx = i*0.1;
        x[i] = 2.0 * k * k / (cosh(k * (xx - 25)) * cosh(k * (xx - 25)));
    }
    rk4(n, from, x, h, to);

    end_time = omp_get_wtime();

    freopen("CON", "w", stdout);
    printf("\nКоличество потоков: %d\n", omp_get_num_threads());
    printf("\nЗатраченное время: %.16g\n", end_time - start_time);
    
    free(x);
    return 0;
}