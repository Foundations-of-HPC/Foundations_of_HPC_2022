#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>


int main(int argc, char** argv) {

  int num_elements = 100;
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int* data = (int*)malloc(sizeof(int) * num_elements);
  assert(data != NULL);

  MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);

  free(data);
  MPI_Finalize();
}
