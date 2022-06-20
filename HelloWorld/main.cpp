#include <iostream>

#include "mpi.h"
#include "omp.h"

void task21(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int rank, myRank, numProcs;
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
#pragma omp parallel private(rank)
	{
		rank = omp_get_thread_num();
		srand(time(NULL) + rank);
		int num = rand();
		printf("I am %d thread in %d process my random number = %d\n", rank, myRank, num);
	}
	MPI_Finalize();
}

int main(int argc, char** argv){
	task21(argc, argv);
	return 0;
}