# Running basic MPI program on ORFEO

---
Author: Niccolo Tosato
---

## List of available program

   - hello_world.c  : program presented in class, realy basic 
   - hello_world.F90: F90 version, useful just to check fortran compilation 
   - hello_world.c : yet another basic hello world with an additional MPI function to get the name of the node 
   - mpi_env_call.c: same as above
   - send_message.F90 : the basic MPI send/recv message presented in class  
   - deadlock.c : deadlock program, a slightly complex version of the one discussed in class 
   - linear-array.c : a simple linear array where each processor receive from left, sum its rank and send to right.     
   - CBlockSends.c  : a program that compares different blocking operations 
   - Brecv.c :  a program that demonstrates that replacement of a blocking receive with a nonblocking receive can reduce the synchronization overhead for a corresponding standard blocking send, for message sizes greater than 16KB.
   - mpi_pi.c : a program that computes pi with montecarlo technique.
   
