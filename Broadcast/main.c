#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mpi.h"

int main(int argc, char** argv)
{
	int size, rank, N = 5, count = 10;
	int* mass = (int*)malloc(N * sizeof(int));
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		srand((unsigned)time(NULL));
		printf_s("process %d initialize mass: ", rank);
		for (int i = 0; i < N; i++)
		{
			mass[i] = rand() % 100;
			printf_s("%d ", mass[i]);
		}
		printf_s("and send using MPI_Send\n");
		MPI_Send(mass, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else if (rank < count) {
		MPI_Status status;
		int* recvValue = (int*)malloc(sizeof(int) * N);
		MPI_Recv(recvValue, N, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		printf_s("process %d from %d is get", rank, rank-1);
		for (int i = 0; i < N; i++)
		{
			printf_s("%d ", recvValue[i]);
		}
		if (rank + 1 < count)
			MPI_Send(recvValue, N, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		free(recvValue);
	}
	free(mass);
	MPI_Finalize();
	return 0;
}