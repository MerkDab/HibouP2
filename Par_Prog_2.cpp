#include <iostream>
#include <ctime>
#include <omp.h>

using namespace std;

const int N = 1000; // Размер матрицы

//Функция перемножения матриц А и В
void multiplyMatrices(int** A, int** B, int** result) {
#pragma omp parallel for collapse(2)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    // Инициализация матриц A и B
    int** A, ** B, ** result;

    // Выделение памяти
    A = new int* [N];
    B = new int* [N];
    result = new int* [N];

    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        result[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;  // Заполнение случайными значениями
            B[i][j] = rand() % 10;
        }
    }

    // Тестирование с разным количеством потоков и различными стратегиями планирования
    double start_time, end_time;
    int num_threads[] = { 1, 2, 4, 8 };
    const char* schedules[] = { "static", "dynamic", "guided" }; // имена стратегий планирования

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            omp_set_num_threads(num_threads[i]);
            // выбор планирования
            if(j == 0)
                omp_set_schedule(omp_sched_static, 1); 
            if(j == 1)
                omp_set_schedule(omp_sched_dinamic, 1);
            if(j == 2)
                omp_set_schedule(omp_sched_guided, 1);
            start_time = omp_get_wtime();
            multiplyMatrices(A, B, result);
            end_time = omp_get_wtime();
            cout << "Threads: " << num_threads[i] << ", Schedule: " << schedules[j] << ", Time: " << end_time - start_time << " seconds\n";
        }
    }

    // Освобождение памяти
    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] result[i];
    }
    delete[] A;
    delete[] B;
    delete[] result;

    return 0;
}