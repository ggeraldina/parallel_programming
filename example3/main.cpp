#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <omp.h>

#define N 2

// стр. 9 Пример 2 http://imas.tpu.ru/lecture/d_e_systems.pdf

typedef double (*Function) (double, double*);

double func_1(double x, double *y) {
    return - 1 - 4 * x  + 4 * y[0] - 5 * y[1];
}

double func_2(double x, double *y) {
    return x  + y[0] - 2 * y[1];
}

void parse_argv(int argc, char *argv[], double *a, double *b, double *h, int *num_threads) {
    if (argc < 5) {
        return;
    }
    // <проверки на ввод...>
    *a = atof(argv[1]);
    *b = atof(argv[2]);
    *h = atof(argv[3]);
    *num_threads = atof(argv[4]);
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
    system("chcp 65001"); // utf-8

    double a = -0.5, b = 0.5, h = 0.05;
    int num_threads = 2;
    parse_argv(argc, argv, &a, &b, &h, &num_threads);
    double x_a = a;
    Function func[N] = {func_1, func_2};     
    double y_a[N] = {10 * exp(3 * a) + exp(- 1 * a), 2 * exp(3 * a) + exp(- 1 * a)};
    double n = (b - a) / h;
    double current_x = x_a;
    double current_y[N];    
    double current_k1[N], current_k2[N], current_k3[N], current_k4[N];

    double start_time = 0., end_time = 0.;
    omp_set_num_threads(num_threads);
    start_time = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        current_y[i] = y_a[i];
        current_k1[i] = 0.;
        current_k2[i] = 0.;
        current_k3[i] = 0.;
        current_k4[i] = 0.;
        // printf(
        //     "\nЗакончил работу поток №%d из %d потоков\n", 
        //     omp_get_thread_num(), 
        //     omp_get_num_threads()
        // );
    }
    
    for (int i = 0; i < n; i++) {
        printf("\nx[%2d] = %3.16g\n", i, current_x);  
        for (int j = 0; j < N; j++) {
            printf("y[%2d, %2d] = %3.16g; ", i, j, current_y[j]);            
        }

        #pragma omp parallel for
        for (int j = 0; j < N; j++) {     
            current_k1[j] = func[j](current_x, current_y);
        // printf(
        //     "\nЗакончил работу поток №%d из %d потоков\n", 
        //     omp_get_thread_num(), 
        //     omp_get_num_threads()
        // );
        }
            
        double current_y_k2[N];
        #pragma omp parallel for
        for (int s = 0; s < N; s++) {
            current_y_k2[s] = current_y[s] + current_k1[s] * h / 2.;
            // printf(
            //     "\nЗакончил работу поток №%d из %d потоков\n", 
            //     omp_get_thread_num(), 
            //     omp_get_num_threads()
            // );
        }
        #pragma omp parallel for
        for (int j = 0; j < N; j++) { 
            current_k2[j] = func[j](current_x + h / 2., current_y_k2);
            // printf(
            //     "\nЗакончил работу поток №%d из %d потоков\n", 
            //     omp_get_thread_num(), 
            //     omp_get_num_threads()
            // );
        }

        double current_y_k3[N];
        #pragma omp parallel for
        for (int s = 0; s < N; s++) {
            current_y_k3[s] = current_y[s] + current_k2[s] * h / 2.;
            // printf(
            //     "\nЗакончил работу поток №%d из %d потоков\n", 
            //     omp_get_thread_num(), 
            //     omp_get_num_threads()
            // );
        }
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            current_k3[j] = func[j](current_x + h / 2., current_y_k3);
            // printf(
            //     "\nЗакончил работу поток №%d из %d потоков\n", 
            //     omp_get_thread_num(), 
            //     omp_get_num_threads()
            // );
        }

        double current_y_k4[N];
        #pragma omp parallel for
        for (int s = 0; s < N; s++) {
            current_y_k4[s] = current_y[s] + current_k3[s] * h;
            // printf(
            //     "\nЗакончил работу поток №%d из %d потоков\n", 
            //     omp_get_thread_num(), 
            //     omp_get_num_threads()
            // );
        }
        #pragma omp parallel for         
        for (int j = 0; j < N; j++) {
            current_k4[j] = func[j](current_x + h, current_y_k4);
            // printf(
            //     "\nЗакончил работу поток №%d из %d потоков\n", 
            //     omp_get_thread_num(), 
            //     omp_get_num_threads()
            // );
        }

        current_x += h;
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            current_y[j] = compute_next_y(
                h, current_y[j], 
                current_k1[j], current_k2[j], 
                current_k3[j], current_k4[j]
            );
            // printf(
            //     "\nЗакончил работу поток №%d из %d потоков\n", 
            //     omp_get_thread_num(), 
            //     omp_get_num_threads()
            // );
        }
    }

    printf("\nx[%2d] = %3.16g\n", (int) n, current_x);
    for (int i = 0; i < N; i++) {
        printf("y[%2d, %2d] = %3.16g; ", (int) n, i, current_y[i]);
    }

    end_time = omp_get_wtime();
    printf("\nЗатраченное время: %.16g\n", end_time - start_time);

    return 0;
}

// Ответ (график)
// Руками интервал нужно будет поправить на [-0,5; 0,5]
// http://www.yotx.ru/#!1/3_h/ubB4yz/QPG2b4RQ/hf2z/aP9g/2PeTUmv7WwfgtfWNC/DeLnRtfWNzC7y3u3%40wT6JhN3ZOGY%40nW4zHrcuL3f2t/R3w2vrGBXhvF7q2vrG5Bd7b3T/YJ9GwGzunjMfTLcbj1uXF7v7WPgI=

// 1 поток
// main.exe -0.5 0.5 0.05 1  
// Active code page: 65001
// x[ 0] = -0.5
// y[ 0,  0] = 3.880022872184426; y[ 0,  1] = 2.094981590996988; 
// x[ 1] = -0.45
// y[ 1,  0] = 4.213733530432488; y[ 1,  1] = 2.065470025593382; 
// x[ 2] = -0.4
// y[ 2,  0] = 4.61541809428561; y[ 2,  1] = 2.05863694082392;   
// x[ 3] = -0.35
// y[ 3,  0] = 5.093685693961315; y[ 3,  1] = 2.075761145502649; 
// x[ 4] = -0.3
// y[ 4,  0] = 5.658655332752327; y[ 4,  1] = 2.118445029262749; 
// x[ 5] = -0.2500000000000001
// y[ 5,  0] = 6.322194547266673; y[ 5,  1] = 2.188661240857599; 
// x[ 6] = -0.2000000000000001
// y[ 6,  0] = 7.098196968316511; y[ 6,  1] = 2.288807197983437; 
// x[ 7] = -0.1500000000000001
// y[ 7,  0] = 8.00290506435827; y[ 7,  1] = 2.421768682497708;  
// x[ 8] = -0.1000000000000001
// y[ 8,  0] = 9.055285366669439; y[ 8,  1] = 2.59099397848601;  
// x[ 9] = -0.05000000000000007
// y[ 9,  0] = 10.27746465732833; y[ 9,  1] = 2.800580247123001;
// x[10] = -6.938893903907228e-17
// y[10,  0] = 11.69523697418166; y[10,  1] = 3.05537410700695;
// x[11] = 0.04999999999999993
// y[11,  0] = 13.33865288229081; y[11,  1] = 3.361088707814041;
// x[12] = 0.09999999999999994
// y[12,  0] = 15.24270431480019; y[12,  1] = 3.724439955909095;
// x[13] = 0.1499999999999999
// y[13,  0] = 17.44812043954799; y[13,  1] = 4.153304981320007;
// x[14] = 0.2
// y[14,  0] = 20.00229250958043; y[14,  1] = 4.656906435941857;
// x[15] = 0.2499999999999999
// y[15,  0] = 22.96034856242927; y[15,  1] = 5.246026794262558;
// x[16] = 0.2999999999999999
// y[16,  0] = 26.38640221008982; y[16,  1] = 5.933257503399342;
// x[17] = 0.3499999999999999
// y[17,  0] = 30.35500368521743; y[17,  1] = 6.733288614029435;
// x[18] = 0.3999999999999999
// y[18,  0] = 34.95282586758724; y[18,  1] = 7.663245435577865; 
// x[19] = 0.4499999999999999
// y[19,  0] = 40.28062331109308; y[19,  1] = 8.743079818341945;
// x[20] = 0.4999999999999999
// y[20,  0] = 46.45550844488056; y[20,  1] = 9.996024895963082;
// Затраченное время: 0.02467730001080781


// 2 потока
// main.exe -0.5 0.5 0.05 2
// Active code page: 65001
// x[ 0] = -0.5
// y[ 0,  0] = 3.880022872184426; y[ 0,  1] = 2.094981590996988;
// x[ 1] = -0.45
// y[ 1,  0] = 4.213733530432488; y[ 1,  1] = 2.065470025593382;
// x[ 2] = -0.4
// y[ 2,  0] = 4.61541809428561; y[ 2,  1] = 2.05863694082392;
// x[ 3] = -0.35
// y[ 3,  0] = 5.093685693961315; y[ 3,  1] = 2.075761145502649;
// x[ 4] = -0.3
// y[ 4,  0] = 5.658655332752327; y[ 4,  1] = 2.118445029262749; 
// x[ 5] = -0.2500000000000001
// y[ 5,  0] = 6.322194547266673; y[ 5,  1] = 2.188661240857599;
// x[ 6] = -0.2000000000000001
// y[ 6,  0] = 7.098196968316511; y[ 6,  1] = 2.288807197983437;
// x[ 7] = -0.1500000000000001
// y[ 7,  0] = 8.00290506435827; y[ 7,  1] = 2.421768682497708;
// x[ 8] = -0.1000000000000001
// y[ 8,  0] = 9.055285366669439; y[ 8,  1] = 2.59099397848601;
// x[ 9] = -0.05000000000000007
// y[ 9,  0] = 10.27746465732833; y[ 9,  1] = 2.800580247123001;
// x[10] = -6.938893903907228e-17
// y[10,  0] = 11.69523697418166; y[10,  1] = 3.05537410700695;
// x[11] = 0.04999999999999993
// y[11,  0] = 13.33865288229081; y[11,  1] = 3.361088707814041;
// x[12] = 0.09999999999999994
// y[12,  0] = 15.24270431480019; y[12,  1] = 3.724439955909095;
// x[13] = 0.1499999999999999
// y[13,  0] = 17.44812043954799; y[13,  1] = 4.153304981320007;
// x[14] = 0.2
// y[14,  0] = 20.00229250958043; y[14,  1] = 4.656906435941857;
// x[15] = 0.2499999999999999
// y[15,  0] = 22.96034856242927; y[15,  1] = 5.246026794262558;
// x[16] = 0.2999999999999999
// y[16,  0] = 26.38640221008982; y[16,  1] = 5.933257503399342;
// x[17] = 0.3499999999999999
// y[17,  0] = 30.35500368521743; y[17,  1] = 6.733288614029435;
// x[18] = 0.3999999999999999
// y[18,  0] = 34.95282586758724; y[18,  1] = 7.663245435577865;
// x[19] = 0.4499999999999999
// y[19,  0] = 40.28062331109308; y[19,  1] = 8.743079818341945;
// x[20] = 0.4999999999999999
// y[20,  0] = 46.45550844488056; y[20,  1] = 9.996024895963082;
// Затраченное время: 0.02055209991522133



