#include <iostream>
#include <Windows.h>

#include "mpi.h"
#include <vector>

#define OPERATION_FINISH 0
#define OPERATION_SUM 1

#define TAG_OPERATION 0
#define TAG_VALUES 1
#define TAG_RESULT 2

void task41(int argc, char** argv, int n) {
	int myRank, numProcs, N = n, count = 10;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Status status;
	MPI_Request request;
	int length = 0;
	char name[100];
	MPI_Get_processor_name(name, &length);
	std::cout << "Process " << myRank << " of " << numProcs << " is running on " << name << std::endl;

	if (myRank == 0)
	{
		srand(time(NULL));
		std::vector<int> mass;
		for (int i = 0; i < N; ++i) {
			mass.push_back(rand() % 10);
			std::cout << mass[i] << " ";
		}
		std::cout << std::endl;
		if (mass.size() % 2 != 0)
			mass.push_back(0);
		int i = 0;
		int process_id = 1;
		process_id = 1;
		while (N != 1) {
			if (mass.size() % 2 != 0)
				mass.push_back(0);
			process_id = 1;
			for (i = 0; i < N / 2 + (N % 2); i++) {
				int operation = OPERATION_SUM;
				MPI_Isend(&operation, 1, MPI_INT, process_id, TAG_OPERATION, MPI_COMM_WORLD, &request);
				MPI_Wait(&request, &status);
				MPI_Isend(&mass[i * 2], 2, MPI_INT, process_id, TAG_VALUES, MPI_COMM_WORLD, &request);
				MPI_Wait(&request, &status);
				process_id = (process_id + 1) % count;
				if (process_id == 0)
					process_id += 1;
			}
			N = N / 2 + (N % 2);
			mass.clear();
			process_id = 1;
			//std::cout << "N = " << N << std::endl;
			for (i = 0; i < N; i++) {
				int result;
				MPI_Irecv(&result, 1, MPI_INT, process_id, TAG_RESULT, MPI_COMM_WORLD, &request);
				MPI_Wait(&request, &status);
				mass.push_back(result);
				process_id = (process_id + 1) % count;
				if (process_id == 0)
					process_id += 1;
			}
			if (N == 1) {
				std::cout << mass[0];
				int operation = OPERATION_FINISH;
				for (i = 1; i < count; i++) {
					MPI_Isend(&operation, 1, MPI_INT, i, TAG_OPERATION, MPI_COMM_WORLD, &request);
					MPI_Wait(&request, &status);
				}
				break;
			}
		}
	}
	else {
		int input[2];
		int result;
		int operation;
		while (true) {
			MPI_Irecv(&operation, 1, MPI_INT, 0, TAG_OPERATION, MPI_COMM_WORLD, &request);
			MPI_Wait(&request, &status);
			if (operation == OPERATION_FINISH) break;
			MPI_Irecv(&input, 2, MPI_INT, 0, TAG_VALUES, MPI_COMM_WORLD, &request);
			MPI_Wait(&request, &status);
			result = input[0] + input[1];
			//std::cout << input[0] << "+" << input[1] << std::endl;
			MPI_Isend(&result, 1, MPI_INT, 0, TAG_RESULT, MPI_COMM_WORLD, &request);
			MPI_Wait(&request, &status);
			//std::cout << "SEND " << result << std::endl;
		}
	}
	//std::cout << "PROCESS " << myRank << " IS STOPPED" << std::endl;
	MPI_Finalize();
}

int main(int argc, char** argv)
{
	clock_t start, end;
	double seconds;
	start = clock();
	task41(argc, argv, 100);
	end = clock();
	seconds = (double)(end - start) / CLOCKS_PER_SEC;
#pragma omp master
	std::cout << "[mpi] time: " << seconds << "s" << std::endl;
}