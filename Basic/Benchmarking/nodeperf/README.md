## Exercise 2: compile and run nodeperf.c program 

Taken from reference 2: Optimizing HPC application with Intel Cluster tools 

### part 1: taken from the book 

Compile the nodeperf program using the Intel MPI compile wrapper script for Intel
compiler with the optimizations enabled to at least -O2 level, tuning for the instruction set
supported on the build machine (either by using the -xHost or by directly specifying another
instruction set target as described in Table 1-1), enabling OpenMP support (-qopenmp), and
linking with the MKL library for the optimized version of the DGEMM routine (-mkl):

```
$ mpiicc -O2 -xHost -qopenmp -mkl nodeperf.c -o nodeperf
```

Successful completion of the command should produce an executable binary
nodeperf in the current working directory. It is ready to run. Just before the run, however,
you need to set a few important environment variables that control the number of
OpenMP threads and their placement on the system:

```
$ export OMP_NUM_THREADS=XX 
$ export OMP_PLACES=cores
```

where XX is the number of core you have on your system.
Report the performance you get on ORFEO node and compare with Peak performance of the node using the whole node and just a single core.


