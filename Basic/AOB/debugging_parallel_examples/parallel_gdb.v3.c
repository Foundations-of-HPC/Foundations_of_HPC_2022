#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <mpi.h>


int main(int argc, char **argv)
{
  int Me, Size;
  
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &Me);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  

#ifdef DEBUGGER
  int   Wait = Me;
  pid_t my_pid;
  char  my_host_name[200];

  gethostname(my_host_name, 200);
  my_pid = getpid();

  for(int i = 0; i < Size; i++)
    {
      if(i == Me)
	printf("task with PID %d on host %s is waiting\n", my_pid, my_host_name); fflush(stdout);
      MPI_Barrier(MPI_COMM_WORLD);
    }
  
  while(!Wait)
    sleep(1);

  MPI_Barrier(MPI_COMM_WORLD);
#endif


  for(int i = 0; i < Size; i++)
    {
      if(i == Me)
	printf("Hi, this is task %d\n", Me); fflush(stdout);
      MPI_Barrier(MPI_COMM_WORLD);
    }

  MPI_Finalize();
  return 0;
}
