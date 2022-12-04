## HPL benchmarks 

## High Performance Linpack (HPL) benchmark

**HPL** is a portable as well as freely available implementation of the High Performance Computing Linpack Benchmark used to measure computer performance in terms of 64-bit floating-opint operations per second or **FLOPS**. This benchmark is used to build the TOP500 list, ranking the world's most powerful supercomputers.

The HPL software requires an implementation of the Message Passing Interface **MPI** and an implementation of Linear Algebra Subprograms, **BLAS**, to run.

In the available tutorial we will compile and use HPL to benchmark Orfeo [Intel](hpl-on-intel.md) ad [Epyc](hpl-on-epyc.md)  nodes using different settings:

* different HPL parameters: N, NB, P, Q
* different MPI implementations: OpenMPI vs IntelMPI
* different BLAS implementation: OpenBlas vs IntelMKL

The objective is to achieve a performance as close to the theoretical peak performance R<sub>peak</sub> as possible.

--------------------------

### Resources

- [HPL tunning](https://www.netlib.org/benchmark/hpl/tuning.html): guideline where they explained the parameters used.
- [HPL parameters diagram](https://www.researchgate.net/profile/Jesus-Labarta/publication/228524393/figure/fig1/AS:301996025368584@1449012871620/An-example-of-P-by-Q-partitioning-of-a-HPL-matrix-in-6-processes-2x3-decomposition.png): An example of P by Q partitioning of a HPL matrix in 6 processes (2x3 decomposition)
- [ulhpc-tutorial](https://ulhpc-tutorials.readthedocs.io/en/latest/parallel/mpi/HPL/): useful reference for these tutorials
