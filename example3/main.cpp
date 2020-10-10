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

// Ответ (график) http://www.yotx.ru/#!1/3_h/ubB4yz/QPG2b4RQ/hf2z/aP9g/2PeTUmv7WwfgtfWNC/DeLnRtfWNzC7y3u3%40wT6JhN3ZOGY%40nW4zHrcuL3f2t/R3w2vrGBXhvF7q2vrG5Bd7b3T/YJ9GwGzunjMfTLcbj1uXF7v7WPgI=