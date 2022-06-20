#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#include "mpi.h"

int main(int argc, char** argv)
{
	int size, rank, N = 5, count = 10;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	MPI_Request request;
	int* recvValue = (int*)malloc(sizeof(int) * N);
	if (rank != 0 && rank < count) {
		Sleep(5000);
		MPI_Irecv(recvValue, recvValue, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		printf_s("\Get %d <- %d using MPI_Irecv\n", rank, rank - 1);
		for (int i = 0; i < N; i++) {
			printf_s("%d ", recvValue[i]);
		}
		for (int i = 0; i < N; i++) {
			recvValue[i] = rand() % 10;
		}
		MPI_Isend(recvValue, N, MPI_INT, (rank + 1), 170, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		printf_s("\nSend %d -> %d using MPI_Isend\n", rank, rank + 1);
		for (int i = 0; i < N; i++) {
			printf_s("%d ", recvValue[i]);
		}
	}
	else if (rank == 0) {
		for (int i = 0; i < N; i++) {
			recvValue[i] = rand() % 10;
		}
		//MPI_Isend(&sendValue, 1, MPI_INT, 1, 170, MPI_COMM_WORLD, &request);
		MPI_Isend(recvValue, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		printf_s("\nSend %d -> %d using MPI_Isend\n", rank, rank + 1);
		for (int i = 0; i < N; i++) {
			printf_s("%d ", recvValue[i]);
		}
	}
	else if (rank == count) {
		Sleep(5000);
		MPI_Irecv(recvValue, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		printf_s("\Get %d <- %d using MPI_Irecv\n", rank, rank - 1);
		for (int i = 0; i < N; i++) {
			printf_s("%d ", recvValue[i]);
		}

	}
	free(recvValue);
	MPI_Finalize();
	return 0;
}