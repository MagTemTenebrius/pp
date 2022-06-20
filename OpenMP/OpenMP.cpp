#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

//
// Task1.3 BEGIN
//
void floyd(const std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>>& result, int n) {
	int i, j, k;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			result[i][j] = matrix[i][j];
		}
	}
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (result[i][j] < result[i][k] + result[k][j])
					result[i][j] = result[i][k] + result[k][j];
			}
		}
	}
}

void floyd_openmp(const std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>>& result, int n, int count) {
	int i, j, k;
	omp_set_num_threads(count);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			result[i][j] = matrix[i][j];
		}
	}

#pragma omp parallel for private(k)
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (result[i][j] < result[i][k] + result[k][j])
					result[i][j] = result[i][k] + result[k][j];
			}
		}
	}
}

// Реализовать параллельный вариант алгоритма Флойда с использованием библиотеки OpenMP.
// Провести сравнительный анализ времени выполнения последовательного и параллельного вариантов алгоритма.
// Пробовать разное количество потоков, составить таблицу результатов.
void task03(int n) {
	clock_t start, end;
	std::vector<std::vector<int>> matrix = std::vector<std::vector<int>>(n, std::vector<int>(n));
	std::vector<std::vector<int>> result = std::vector<std::vector<int>>(n, std::vector<int>(n));
	double seconds;
	// std::cout << "Clear floyd start" << std::endl;
	// start = clock();
	// floyd(matrix, result, n);
	// end = clock();
	// seconds = (double)(end - start) / CLOCKS_PER_SEC;
	// std::cout << "[clear floyd] time: " << seconds << "s" << std::endl;


	// std::vector<int> thread_count = { 1, 2, 3, 4, 6, 8, 12, 16, 32 };
	std::vector<int> thread_count = { 6};

	for (int threads : thread_count) {
		std::cout << "OpenMP alg start threads: " << threads << std::endl;
		start = clock();
		floyd_openmp(matrix, result, n, threads);
		end = clock();
		seconds = (double)(end - start) / CLOCKS_PER_SEC;
		std::cout << "[openmp] time: " << seconds << "s, threads = " << threads << std::endl;
	}

}
//
// Task1.3 END
//

// 
// Task1.2 BEGIN
//
void mult(std::vector<std::vector<int>>& matrixA, std::vector<std::vector<int>>& matrixB, std::vector<std::vector<int>>& matrixC, const int n) {
	int i = 0, j = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			int result = 0;
			for (int k = 0; k < n; k++) {
				result += matrixA[i][k] * matrixB[k][j];
			}
			matrixC[i][j] = result;
		}
	}
}

void mult_openmp(std::vector<std::vector<int>>& matrixA, std::vector<std::vector<int>>& matrixB, std::vector<std::vector<int>>& matrixC, const int n, const int count) {
	omp_set_num_threads(count);
	int i = 0, j = 0;

#pragma omp parallel for private(i, j)
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			int result = 0;
			for (int k = 0; k < n; k++) {
				result += matrixA[i][k] * matrixB[k][j];
			}
			matrixC[i][j] = result;
		}
	}
}

// Реализовать параллельное перемножение матриц с использованием библиотеки OpenMP.
// Провести сравнительный анализ времени выполнения последовательного и параллельного вариантов алгоритма.
// Пробовать разное количество потоков, составить таблицу результатов.
void task02(int n) {
	std::vector<std::vector<int>> matrixA = std::vector<std::vector<int>>(n, std::vector<int>(n));
	std::vector<std::vector<int>> matrixB = std::vector<std::vector<int>>(n, std::vector<int>(n));
	std::vector<std::vector<int>> matrixC = std::vector<std::vector<int>>(n, std::vector<int>(n));
	clock_t start, end;
	double seconds;
	// std::cout << "Clear alg start" << std::endl;
	// start = clock();
	// mult(matrixA, matrixB, matrixC, n);
	// end = clock();
	// seconds = (double)(end - start) / CLOCKS_PER_SEC;
	// std::cout << "[clear] time: " << seconds << "s" << std::endl;

	// std::vector<int> thread_count = { 1, 2, 3, 4, 6, 8, 12, 16, 32 };
	std::vector<int> thread_count = {6};

	for (int thread_id : thread_count) {
		std::cout << "OpenMP alg start thread_id: " << thread_id << std::endl;
		start = clock();
		mult_openmp(matrixA, matrixB, matrixC, n, thread_id);
		end = clock();
		seconds = (double)(end - start) / CLOCKS_PER_SEC;
		std::cout << "[openmp] time: " << seconds << "s, threads = " << thread_id << std::endl;
	}

}
//
// Task1.2 END
//

//
// Task1.1 BEGIN
// 
// Каждый поток генерирует рандомное число, выводит его в консоль и записывает в файл rank.txt.
// rank в данном случае - номер потока
// Использовать srand + rand.
void task01() {
	int size, rank;
	omp_set_num_threads(5);
#pragma omp parallel private(size, rank)
	{
		rank = omp_get_thread_num();
		srand(time(NULL) + rank);
		int num = rand();
		std::ofstream fout(std::to_string(rank) + ".txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt
		fout << num; // запись строки в файл
		printf("Rank %d => %d\n", rank, num);
		fout.close(); // закрываем файл

	}
}
//
// Task1.1 END
//

int main() {
	std::vector<int> sizes = {100, 1000, 5000, 10000};
	task01();
	for (int n : sizes) {
		std::cout << "Start SIZE = " << n << std::endl;
		task02(n);
		task03(n);
	}
	system("pause");
	return 0;
}