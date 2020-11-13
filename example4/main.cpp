#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define M_PI 3.14159265358979323846

#define H 10
#define HH H*H
#define HHH H*H*H

void printfArray(double *d, int n, double t) {
    printf("t = %.8f\n", t);
    for (int i = 0; i < n; i++) {
        printf("%.7f ", d[i]);
    }
    printf("\n");
}

void f(double t, double *x, double *dxdt, int n) {
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            double x_minus_1 = x[n - 1];
            double x_minus_2 = x[n - 2];
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x_minus_1) * 0.5 * H) 
                      - ((x[i + 2] - 2 * x[i + 1] + 2 * x_minus_1 - x_minus_2) * 0.5 * HHH);
        } else if (i == 1) {
            double x_minus_2 = x[n - 1];
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x[i - 1]) * 0.5 * H) 
                      - ((x[i + 2] - 2 * x[i + 1] + 2 * x[i - 1] - x_minus_2) * 0.5 * HHH);
        } else if (i == n-2) {
            double x_plus_2 = x[0];
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x[i - 1]) * 0.5 * H) 
                      - ((x_plus_2 - 2 * x[i + 1] + 2 * x[i - 1] - x[i - 2]) * 0.5 * HHH);        
        } else if (i == n-1) {
            double x_plus_1 = x[0];
            double x_plus_2 = x[1];
            dxdt[i] = - 6 * x[i] * ((x_plus_1 - x[i - 1]) * 0.5 * H) 
                      - ((x_plus_2 - 2 * x_plus_1 + 2 * x[i - 1] - x[i - 2]) * 0.5 * HHH);
        } else {
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x[i - 1]) * 0.5 * H) 
                      - ((x[i + 2] - 2 * x[i + 1] + 2 * x[i - 1] - x[i - 2]) * 0.5 * HHH);
        }
    }
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
        printf("-----------------\n");

        //k1
        f(t, x, k1, n);
        //k2
		//
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + 0.5 * h * k1[i];
        }
        f(t + 0.5 * h, temp, k2, n);
        //k3
		//
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + 0.5 * h * k2[i];
        }
        f(t + 0.5 * h, temp, k3, n);
        //k4
		//
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + h * k3[i];
        }
        f(t + h, temp, k4, n);
        //res
        for (int i = 0; i < n; i++) {
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
    freopen("output.txt", "w", stdout);
    int n = 50*H; // amount x
    double h = 0.001; // step t (time)
    double *x = (double*) malloc(n * sizeof (double));
    double from = 0.0, to = 10.0; // t (time)
    clock_t start, finish;

    double k = 1.0;
    for (int i = 0; i < n; i++)
    {
        double xx = i*0.1;
        x[i] = 2.0 * k * k / (cosh(k * (xx - 25)) * cosh(k * (xx - 25)));
    }

    start = clock();
    rk4(n, from, x, h, to);
    finish = clock();
    printf("time: %f seconds.\n", ((float) (finish - start)) / CLOCKS_PER_SEC);

    free(x);
    return 0;
}

// график = 2 / (ch(x -4*t) * ch(x-4*t)), t=1
// http://yotx.ru/#!1/3_h/ubWwf7Wwf7Rgzhf23/aP9g/2DfT0qt7e9sb/zyN/Y2QeCtXfAvf2NvEwTe2t3dP9gn0bAbO6eMx9MtxuPW5cXu/tY%40Ag==
