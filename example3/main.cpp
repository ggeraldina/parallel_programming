// решение дифференциального уравнения
// y' = y * cos(x), y(0) = 1
// методом Рунге-Кутты

#include <iostream>
#include <cmath>
#include <stdlib.h>

double func(double x, double y) {
    // Уравнение: y' = y * cos(x)
    // Решение: y = e^sin(x) + C
    // 
    // Задача Коши: y' = y * cos(x), y(0) = 1
    // Решение: y = e^sin(x)
    return y * cos(x);
}

void parse_argv(int argc, char *argv[], double *a, double *b, double *h) {
    if (argc < 4) {
        return;
    }
    // <проверки на ввод...>
    *a = atof(argv[1]);
    *b = atof(argv[2]);
    *h = atof(argv[3]);
}

double compute_next_y(
        double h, 
        double y, 
        double k1, 
        double k2, 
        double k3, 
        double k4
    ) {
    return y + h * (k1 + 2. * k2 + 2. * k3 + k4) / 6.;
}

int main(int argc, char *argv[]) {
    double a = -10., b = 10., h = 1.;
    parse_argv(argc, argv, &a, &b, &h);
    double x_a = a, y_a = exp(sin(a));
    double n = (b - a) / h;
    double current_x = x_a, current_y = y_a;
    double current_k1 = 0., current_k2 = 0., current_k3 = 0., current_k4 = 0.;        
    
    for (int i = 0; i < n; i++) {
        printf("x[%2d] = %3.16g; y[%2d] = %3.16g;\n", i, current_x, i, current_y);
        current_k1 = func(current_x, current_y);
        current_k2 = func(current_x + h / 2., current_y + current_k1 * h / 2.);
        current_k3 = func(current_x + h / 2., current_y + current_k2 * h / 2.);
        current_k4 = func(current_x + h, current_y + current_k3 * h);
        current_x += h;
        current_y = compute_next_y(h, current_y, current_k1, current_k2, current_k3, current_k4);
    }
    
    printf("x[%2d] = %3.16g; y[%2d] = %3.16g;\n", (int) n, current_x,  (int) n, current_y);
    return 0;
}