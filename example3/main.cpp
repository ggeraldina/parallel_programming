// решение дифференциального уравнения
// y' = y * cos(x), y(0) = 1
// методом Рунге-Кутты

#include <iostream>
#include <cmath>

double func(double x, double y) {
        // Уравнение: y' = y * cos(x)
        // Решение: y = e^sin(x) + C
        // 
        // Задача Коши: y' = y * cos(x), y(0) = 1
        // Решение: y = e^sin(x)
        return y * cos(x);
}

int main(int argc, char *argv[]) {
        double a = -10, b = 10, h = 1;
        double x_a = a, y_a = exp(sin(a));
        double n = (b - a) / h;
        double current_x = x_a, current_y = y_a;
        double current_k1 = 0., current_k2 = 0., current_k3 = 0., current_k4 = 0.;        
        
        for (int i = 0; i < n; i++) {
            printf("x[%2d] = %3.16g; y[%2d] = %3.16g;\n", i, current_x, i, current_y);
            current_k1 = func(current_x, current_y);
            current_k2 = func(current_x + h / 2., current_y + current_k1 / 2.);
            current_k3 = func(current_x + h / 2., current_y + current_k2 / 2.);
            current_k4 = func(current_x + h, current_y + current_k3);
            current_x += h;
            current_y = current_y + h * (current_k1 + 2. * current_k2 + 2. * current_k3 + current_k4) / 6.;
        }
        
        printf("x[%2d] = %3.16g; y[%2d] = %3.16g;\n", (int) n, current_x,  (int) n, current_y);
        return 0;
}

// Вывод программы
// x[ 0] = -10; y[ 0] = 1.722921008021756;
// x[ 1] =  -9; y[ 1] = 0.6663690574505339;
// x[ 2] =  -8; y[ 2] = 0.372543033802918;
// x[ 3] =  -7; y[ 3] = 0.5190300450052782;
// x[ 4] =  -6; y[ 4] = 1.318168323898321;
// x[ 5] =  -5; y[ 5] = 2.594313748243349;
// x[ 6] =  -4; y[ 6] = 2.118248968159429;
// x[ 7] =  -3; y[ 7] = 0.8641951963742864;
// x[ 8] =  -2; y[ 8] = 0.3993288471304583;
// x[ 9] =  -1; y[ 9] = 0.427343083041123;
// x[10] =   0; y[10] = 0.9878525070714881;
// x[11] =   1; y[11] = 2.283534271300043;
// x[12] =   2; y[12] = 2.443731733231433;
// x[13] =   3; y[13] = 1.129203887989189;
// x[14] =   4; y[14] = 0.4606883281408389;
// x[15] =   5; y[15] = 0.3761505625093302;
// x[16] =   6; y[16] = 0.7403095325803766;
// x[17] =   7; y[17] = 1.880146602529533;
// x[18] =   8; y[18] = 2.619435842796266;
// x[19] =   9; y[19] = 1.464432606551309;
// x[20] =  10; y[20] = 0.5663942636858844;

// Должно быть: y = e^sin(x)
// График: http://www.yotx.ru/#!1/3_h/ubWwf7Wwf7Rgzhf23/aP9g/2DfT0qt7a%40t/wPhG3u7%40wf7JBp2Y%40eU8Xi6xXjcurzY3d/aBwU=
// x	y
// -10	1.72
// -9	0.66
// -8	0.37
// -7	0.52
// -6	1.32
// -5	2.61
// -4	2.13
// -3	0.87
// -2	0.4
// -1	0.43
// 0	1
// 1	2.32
// 2	2.48
// 3	1.15
// 4	0.47
// 5	0.38
// 6	0.76
// 7	1.93
// 8	2.69
// 9	1.51
// 10	0.58