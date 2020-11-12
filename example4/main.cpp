#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define M_PI 3.14159265358979323846

void printfArray(double *d, int lenght, double t) {
    printf("t = %.3f \n", t);
    for (int i = 0; i < lenght; i++) {
        printf("%.7f ", d[i]);
    }
    printf("\n");
}

void f(double a, double b, double t, double *x, double *dxdt, int n, int h) {
	// f
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            double x_minus_1 = - 2 / (cosh((a - 1 / n) - 4 * t) * cosh((a - 1 / n) - 4 * t));
            double x_minus_2 = - 2 / (cosh((a - 2 / n) - 4 * t) * cosh((a - 2 / n) - 4 * t));
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x[i]) / h) 
                  - ((x[i + 2] - 2 * x[i + 1] + 2 * x_minus_1 - x_minus_2) / (2 * pow(h, 3)));
        } else if (i == 1) {
            double x_minus_2 = - 2 / (cosh((a - 2 / n) - 4 * t) * cosh((a - 2 / n) - 4 * t));
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x[i]) / h) 
                  - ((x[i + 2] - 2 * x[i + 1] + 2 * x[i - 1] - x_minus_2) / (2 * pow(h, 3)));
        } else if (i == n-2) {
            double x_plus_2 = - 2 / (cosh((b + 2 / n) - 4 * t) * cosh((b + 2 / n) - 4 * t));
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x[i]) / h) 
                  - ((x_plus_2 - 2 * x[i + 1] + 2 * x[i - 1] - x[i - 2]) / (2 * pow(h, 3)));
        } else if (i == n-1) {
            double x_plus_1 = - 2 / (cosh((b + 1 / n) - 4 * t) * cosh((b + 1 / n) - 4 * t));
            double x_plus_2 = - 2 / (cosh((b + 2 / n) - 4 * t) * cosh((b + 2 / n) - 4 * t));
            dxdt[i] = - 6 * x[i] * ((x_plus_1 - x[i]) / h) 
                  - ((x_plus_2 - 2 * x_plus_1 + 2 * x[i - 1] - x[i - 2]) / (2 * pow(h, 3)));
        } else {
            dxdt[i] = - 6 * x[i] * ((x[i + 1] - x[i]) / h) 
                      - ((x[i + 2] - 2 * x[i + 1] + 2 * x[i - 1] - x[i - 2]) / (2 * pow(h, 3)));
        }
    }
}

int rk4(double a, double b, int n, double t, double *x, double h, double finish) {

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
        printf("\n");

        //k1
        f(a, b, t, x, k1, n, h);
        //k2
		//
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + 0.5 * h * k1[i];
        }
        f(a, b, t + 0.5 * h, temp, k2, n, h);
        //k3
		//
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + 0.5 * h * k2[i];
        }
        f(a, b, t + 0.5 * h, temp, k3, n, h);
        //k4
		//
        for (int i = 0; i < n; i++) {
            temp[i] = x[i] + h * k3[i];
        }
        f(a, b, t + h, temp, k4, n, h);
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
    double a = 0, b = 9; // x
    int n = 10; // amount x
    double h = 1; // step t (time)
    double *x = (double*) malloc(n * sizeof (double));
    double from = 1.0, to = 2.0; // t (time)
    clock_t start, finish;

    printf("rk4\n");

    for (int i = 0; i < n; i++) {
        x[i] = - 2 / (cosh((b-a) * i / n - 4) * cosh((b-a) * i / n - 4));
    }

    start = clock();
    rk4(a, b, n, from, x, h, to);
    finish = clock();
    printf("time: %f seconds.\n", ((float) (finish - start)) / CLOCKS_PER_SEC);

    free(x);
    return 0;
}

// график - 2 / (ch(x -4*t) * ch(x-4*t)), t=1
// http://yotx.ru/#!1/3_h/ubWwf7Wwf7Rgzhf23/aP9g/2DfT0qt7W/ubG/88jf2NkHgrV3wL39jbxME3trd3T/YJ9GwGzunjMfTLcbj1uXF7v7WPgI=
