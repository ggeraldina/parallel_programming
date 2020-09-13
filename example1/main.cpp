#include <stdio.h>
#include <cstdlib>

#include "windows.h"

#include <omp.h>

int main(int argc, char *argv[])
{
    std::system("chcp 65001"); // utf-8
    double start_time, end_time, tick;
    start_time = omp_get_wtime();
    Sleep(1000);
    end_time = omp_get_wtime();
    tick = omp_get_wtick();
    printf("Время на замер времени %.16g\n", end_time-start_time);
    printf("Точность таймера %.16g\n", tick);
} 