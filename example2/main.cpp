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
        double x_a = a, y_a = 1.72; // y(0) = 1, когда y(-10) = 1.72
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
// x[ 0] = -10; y[ 0] = 1.72;
// x[ 1] =  -9; y[ 1] = 0.6652393078257974;
// x[ 2] =  -8; y[ 2] = 0.3719114313178817;
// x[ 3] =  -7; y[ 3] = 0.5181500911838701;
// x[ 4] =  -6; y[ 4] = 1.315933525999749;
// x[ 5] =  -5; y[ 5] = 2.589915397283386;
// x[ 6] =  -4; y[ 6] = 2.11465772851509;
// x[ 7] =  -3; y[ 7] = 0.8627300560171718;
// x[ 8] =  -2; y[ 8] = 0.3986518324789703;
// x[ 9] =  -1; y[ 9] = 0.4266185735785337;
// x[10] =   0; y[10] = 0.9861777204248375;
// x[11] =   1; y[11] = 2.279662810047108;
// x[12] =   2; y[12] = 2.439588676200636;
// x[13] =   3; y[13] = 1.127289456857605;
// x[14] =   4; y[14] = 0.4599072857739724;
// x[15] =   5; y[15] = 0.3755128438876623;
// x[16] =   6; y[16] = 0.7390544256583631;
// x[17] =   7; y[17] = 1.876959037178308;
// x[18] =   8; y[18] = 2.614994900307515;
// x[19] =   9; y[19] = 1.461949834926177;
// x[20] =  10; y[20] = 0.5654340094548426;

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