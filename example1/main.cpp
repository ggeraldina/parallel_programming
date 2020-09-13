#include <iostream>
#include <omp.h>

#define M_PI 3.14159265358979323846

double func(double x) {
    return sin(x);
}

int main(int argc, char *argv[]) {
    system("chcp 65001"); // utf-8
    
    double start_time = 0., end_time = 0.;    
    double a = 0., b = 2. * M_PI, result = 0.;
    int number_steps = 100000000;
    double h = (b - a) / number_steps;

    omp_set_num_threads(2);

    start_time = omp_get_wtime();

#pragma omp parallel reduction(+: result)
    {
#pragma omp for
        for (int i = 1; i < number_steps; i++)
            result += func(a + i * h);
        printf(
            "Закончил работу поток №%d из %d потоков\n", 
            omp_get_thread_num(), 
            omp_get_num_threads()
        );
    }

    end_time = omp_get_wtime();

    printf("Интеграл равен: %.16g\n", result);
    printf("Затраченное время: %.16g\n", end_time - start_time);
}

// Без опции /openmp
// Active code page: 65001
// Закончил работу поток №0 из 1 потоков
// Интеграл равен: 7.034712773334064e-09
// Затраченное время: 3.630403299990576

// С опцией /openmp и 4 потоками
// Active code page: 65001
// Закончил работу поток №2 из 4 потоков
// Закончил работу поток №0 из 4 потоков
// Закончил работу поток №1 из 4 потоков
// Закончил работу поток №3 из 4 потоков
// Интеграл равен: -3.725290298461914e-09
// Затраченное время: 0.9987982999882661

// 2 потока
// Active code page: 65001
// Закончил работу поток №1 из 2 потоков
// Закончил работу поток №0 из 2 потоков
// Интеграл равен: 0
// Затраченное время: 1.816816700011259