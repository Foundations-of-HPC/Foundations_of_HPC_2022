# HPL benchmark on Epyc node

### tutorial prepared by Niccolo Tosato.

## Download and build
 
 Download and extract into your home:
 ```
  wget https://netlib.org/benchmark/hpl/hpl-2.3.tar.gz

 ```
 
 Extract:
 ```
 tar -xzvf hpl-2.3.tar.gz

 ```
 
 ### Makefile setup 
 
 Enter in HPL root directory: 
 ```
 cd hpl-2.3
 ```
 
 And copy a sample Makefile to the hpl root directory:
 ```
 cp setup/Make.Linux_PII_CBLAS Make.epyc
 ```
 
 Edit the file `Make.epyc` as follow:
 
 ```
 ARCH         = epyc           
 ```
 
 Set HPL top directory:
 ```
 TOPdir       = $(HOME)/hpl-2.3
 ```
 
 Setup **MPI** environment using the environment variables provided by the module system:
 ```
MPdir        = 
MPinc        = -I$(MPI_INCLUDE)
MPlib        = $(MPI_LIB)/libmpi.so
 ```
 
 Setup **BLAS** environment using again the environment variables provided by the modules system:
 ```
LAdir        = 
LAinc        = -I$(OPENBLAS_INCLUDE)
LAlib        = $(OPENBLAS_LIB)/libopenblas.a
 ```
 
 And finally select the correct compiler wrapper and optimizations:
 ```
CC           = mpicc
CCNOOPT      = $(HPL_DEFS)
CCFLAGS      = $(HPL_DEFS) -fomit-frame-pointer -O3 -funroll-loops -fopenmp
#
# On some platforms,  it is necessary  to use the Fortran linker to find
# the Fortran internals used in the BLAS library.
#
LINKER       = mpicc
LINKFLAGS    = $(CCFLAGS)

 ```
 
 ### Build
 
Allocate an epyc node `salloc -n 1 -N1 -p FHPC --time=1:0:0 --mem=490G`.
 
And build your software after module load:
 ```
[ntosato@login02 hpl-2.3]$ module load architecture/AMD
[ntosato@login02 hpl-2.3]$ module load openMPI/4.1.4/gnu/12.2.1
[ntosato@login02 hpl-2.3]$ module load openBLAS/0.3.21 
[ntosato@login02 hpl-2.3]$ srun -n1 make arch=epyc
 ```
 
 You can find the executables at: `./bin/epyc`.

### SLURM script

Since large matrix require a lot of runtime is usefull to run HPL using a script and submit it and wait for the results.
Once the the HPL is compiled and linked, write the correct appfile (if running an hybrid benchmark) and setup a bash script to submit:

**run.sh**
```
#!/bin/bash
#SBATCH --partition=FHPC
#SBATCH --job-name=my_big_HPL
#SBATCH --nodes=1
#SBATCH --ntasks-per-node 128
#SBATCH --mem=490G
#SBATCH --time=02:00:00


module load architecture/AMD
module load openMPI/4.1.4/gnu
module load openBLAS/0.3.21-omp

export codes=/u/path_to/hpl-2.3/bin/epyc

mpirun -np 128 --map-by core $codes/xhpl
```

Send it to the scheduler with `sbatch run.sh`
 
 ## HPL.dat
 
 `HPL.dat` file details are described <a href="https://netlib.org/benchmark/hpl/tuning.html"> here </a>.
 
 An initial starting point to `HPL.dat` can be generated at <a href=https://www.advancedclustering.com/act_kb/tune-hpl-dat-file/> this link </a>.
 
 HPL config file has a lot of parameter, we will cover only the foundamentals one,following <a href=https://netlib.org/benchmark/hpl/faqs.html> HPL faq </a>.

### Problem size `N`:

The parameter `N` define the size of the coefficient matrix. A good choice for the epyc node can be aroud `200000`.

```
As a rule of thumb, 80 % of the total amount of memory is a good guess. If the problem size you pick is too large, swapping will occur, and the performance will drop. If multiple processes are spawn on each node (say you have 2 processors per node), what counts is the available amount of memory to each process.
```

### Block size `NB`:

Is the size of the block that will be distributed. 

```
HPL uses the block size NB for the data distribution as well as for the computational granularity. From a data distribution point of view, the smallest NB, the better the load balance. You definitely want to stay away from very large values of NB. From a computation point of view, a too small value of NB may limit the computational performance by a large factor because almost no data reuse will occur in the highest level of the memory hierarchy. The number of messages will also increase. Efficient matrix-multiply routines are often internally blocked. Small multiples of this blocking factor are likely to be good block sizes for HPL. The bottom line is that "good" block sizes are almost always in the [32 .. 256] interval. The best values depend on the computation / communication performance ratio of your system. 
```

### Grid shape:

The shape of the communication grid is defined by `P` and `Q`, the number of MPI workers spawned must be `>=P*Q`. Try to avoid *flat* grid. 

```
This depends on the physical interconnection network you have. Assuming a mesh or a switch HPL "likes" a 1:k ratio with k in [1..3]. In other words, P and Q should be approximately equal, with Q slightly larger than P. Examples: 2 x 2, 2 x 4, 2 x 5, 3 x 4, 4 x 4, 4 x 6, 5 x 6, 4 x 8 ... If you are running on a simple Ethernet network, there is only one wire through which all the messages are exchanged. On such a network, the performance and scalability of HPL is strongly limited and very flat process grids are likely to be the best choices: 1 x 4, 1 x 8, 2 x 4 ...
```

## Theoretical performance.

The single node theretical peak performance measured in FLOP per second is:
$$FLOPs=\text{cores}\times \text{frequency} \times{FLOP}/\text{cycle}$$

A single core can deliver up to 16 floating point operations per cycle, since we have 128 cores and the maximum frequency is 2.6 GHz, the theretical peak performance is: 5.3 TFLOPs.

## First attemp to run: full MPI with small problem.

We can try to use 128 mpi processes, using a communication grid with shape `P=8,Q=16`. 

Let's try to solve a *small* system with `N=10000` and `NB=1000` understand how crucial is the process binding.

```
[ntosato@login02 small]$ mpirun -np 128 --map-by core xhpl 
...
...

================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR11C2R4       10000    50     8    16               0.52             1.2933e+03
HPL_pdgesv() start time Thu Dec  1 16:25:39 2022

HPL_pdgesv() end time   Thu Dec  1 16:25:40 2022
...
...
```
The result that we achieved is not enought for us, but anyway is a nice number, about 1.3 Tflops. 

### P and Q

Now let's try to perform the same experiment in the wrong way **mapping by socket**.

The result is *embarrassing*, we are 3 order of magnitude slower:
 
 ```
 ================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR11C2R4       10000    50     8    16            2167.15             3.0769e-01

 ```
 

But let's try to tune the processes grid using a flatten structure with `P=1` and `Q=128` and `NB=50`. Doing this small tuning we solve the problem one order of magnitude faster. But still an awfull results, this experiment shows only how the tuning is importante. 

```
================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR11C2R4       10000    50     1   128             536.28             1.2434e+00

```


And we can try to reduce the amount of communication calls increasing the block size to `NB=200` halving the runtime. 

```
================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR11C2R4       10000   200     1   128             220.34             3.0263e+00

```

 
## Increase problem size: N

Increasing the parallel part let us to scale better, the try to solve a bigger problem, with `N` around 200000, the coefficient matrix is about `300 GB`. 
Once problem size is increased we must find a new block size suitable for the problem. We can try more `Ns` for each run.

The results are nice:

```
================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR11C2R4      200000   160     8    16            1612.57             3.3074e+03
```

We can tune better the HPL.dat file using different broadcast algorithms and different `NBs`.

## Hybrid approach.

Since HPL exploits BLAS library to perform basic calculations, we can exploit shared memory approach when we call BLAS routines and then openMP support.Doing so we reduce the MPI worker and each worker spawn multiple threads. 

First of all we need to load the module `openBLAS/0.3.21-omp`, this version of `openBLAS` use the openMP support to multithreading. This means that the number of threads, affinity and usual settings of openMP still valid. Also the plain version of the openBLAS library has its multithreading support, but it doesn't support affinity, then is not suitable for this application. 

### One MPI process for each CCX

During this experiment we will bind one MPI process to each CCX, and each MPI process will spawn 4 threads filling its own CCX. 
We will spawn `32` processes with `P=4` and `Q=8`. 

We must take care to thread affinity and try to put all children thread togeter inside the CCX.

To achieve this hybrid binding we will use the openMPI *appfile*, is a file from which MPI reads the options for each single process that will spawn.

This setup let us the possibility to forward different environment variables to each process and different binding options. 

To launch openMPI we will use `mpirun --app myappfile`

And the appfile will look like this:

```
-np 1 --bind-to cpu-list --cpu-set 0,1,2,3 -x OMP_NUM_THREADS=4 -x OMP_PLACES={0}:4:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 4,5,6,7 -x OMP_NUM_THREADS=4 -x OMP_PLACES={4}:4:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 8,9,10,11 -x OMP_NUM_THREADS=4 -x OMP_PLACES={8}:4:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 12,13,14,15 -x OMP_NUM_THREADS=4 -x OMP_PLACES={12}:4:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 16,17,18,19 -x OMP_NUM_THREADS=4 -x OMP_PLACES={16}:4:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
...
...
```

Each line will spawn one single process with associated environment varaibles. 

Using the knoledge achieved with `lstopo` about CCX, we know that the first core of each CCX is a multiple of 4, starting from zero, then each MPI worker will reside in a precise set of contiguous cores.

The set of cores available to the MPI process is indicated with the options `--cpu-set <comma sep list>` and the binding rule is then `--bind-to cpu-list`, doing so each MPI process has acces to this set of places, that we propagate to openMP with the environment varible `OMP_PLACES`, that is exported wit `-x`.

To be sure that each core is in the right places, we set `OMP_DISPLAY_AFFINITY=true`. 

If the `cpu-set` list is not specified, the child threads will reside in the same core as the MPI father and this is an unwanted behaviour. 


#### Bonus script to build appfile

generate.sh:
```
#!/bin/bash

stride=${1:-4}

for i in $(seq 0 $stride 127)
do
end=$(($i + $stride - 1))
echo "-np 1 --bind-to cpu-list --cpu-set "$(seq -s ',' $i 1 $end)" -x OMP_NUM_THREADS=$stride -x OMP_PLACES={$i}:$stride:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl"
done
```


Run it with the core stride as first argument (default stride is 4):

```
[ntosato@login02 hybrid]$ ./generate.sh 8
-np 1 --bind-to cpu-list --cpu-set 0,1,2,3,4,5,6,7 -x OMP_NUM_THREADS=8 -x OMP_PLACES={0}:8:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 8,9,10,11,12,13,14,15 -x OMP_NUM_THREADS=8 -x OMP_PLACES={8}:8:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 16,17,18,19,20,21,22,23 -x OMP_NUM_THREADS=8 -x OMP_PLACES={16}:8:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 24,25,26,27,28,29,30,31 -x OMP_NUM_THREADS=8 -x OMP_PLACES={24}:8:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 32,33,34,35,36,37,38,39 -x OMP_NUM_THREADS=8 -x OMP_PLACES={32}:8:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 40,41,42,43,44,45,46,47 -x OMP_NUM_THREADS=8 -x OMP_PLACES={40}:8:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
-np 1 --bind-to cpu-list --cpu-set 48,49,50,51,52,53,54,55 -x OMP_NUM_THREADS=8 -x OMP_PLACES={48}:8:1 -x OMP_PROC_BIND=close -x OMP_DISPLAY_AFFINITY=true xhpl
...
```

Setting a stride of 8 cores we use only one MPI process for each CCD.

#### Results:

```
================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR23C2R4      200000   160     4     8            1705.21             3.1277e+03
```




### One MPI process for each CCD

Mapping only one MPI process for each CCD require to increase the number of spawned thread per process up to 8 and to change the communication grid setting `P=4` and `Q=4`. 

To run this experiment we have to generate an appfile with a stride of 8 cores and update HPL.dat

```
================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR23C2R4      200000   160     4     4            1727.58             3.0872e+03
```


### SLURM script

Since large matrix require a lot of runtime is usefull to run HPL using a script and submit it and wait for the results.  
Once the the HPL is compiled and linked, write the correct appfile (if running an hybrid benchmark) and setup a bash script to submit:

**run.sh**
```
#!/bin/bash 
#SBATCH --partition=FHPC
#SBATCH --job-name=my_big_HPL
#SBATCH --nodes=1
#SBATCH --ntasks-per-node 128
#SBATCH --mem=490G 
#SBATCH --time=02:00:00


module load architecture/AMD
module load openMPI/4.1.4/gnu
module load openBLAS/0.3.21-omp

mpirun -np 128 --map-by core xhpl
```

Send it to the scheduler with `sbatch run.sh`

## Multiple nodes

Since HPL use MPI and distributed memory paradigm, we can run the benchmark across multiple nodes. 

To run the benchmark across multiple node we have only to request at least one more node and to adjust the HPL.dat file, remember that you can solve bigger problems, since the total memory available increase.

This result is with a `N=300000` and show 6.4 TFlops using 256 cores.
```
================================================================================
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR11C2R4      300000   200     8    32            2813.76             6.3972e+03

```


### Comments:

All previous results can be improved, using first a better BLAS library, try the official <a href="https://github.com/amd/blis"> AMD </a> library and the famous MKL from Intel.

Others improvements can be reaced using different compiler, optimizations flags and a profiling session to fine tune HPL.dat.
