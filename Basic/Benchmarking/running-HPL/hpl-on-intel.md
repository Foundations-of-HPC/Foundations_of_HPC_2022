# HPL benchmark using MKL multithread library

The aim of this exercise is to compile the hpl benchmark against the mkl libraries and to tune it in order to get close to the theoretical peak performance of a node of the Ulysses cluster.
Exercise could also be performed on personal laptop/desktop and/or any computational resource you may have access.

## First part: Downloading and installing  HPL

Get the package and unpack it in some location on your home directory 

```
 wget http://www.netlib.org/benchmark/hpl/hpl-2.3.tar.gz
 tar -xvzf hpl-2.3.tar.gz
 cd hpl-2.3
```

Take a look at the INSTALL file in the top dir:

```
>less INSTALL
==============================================================
 High Performance Computing Linpack Benchmark (HPL)
 HPL - 2.3 - December 2, 2018
==============================================================

 1) Retrieve the tar file, then

    gunzip hpl.tgz; tar -xvf hpl.tar

 this  will create an  hpl  directory,  that we call below the
 top-level directory.

 2) Create a file Make.<arch> in the  top-level directory. For
 this purpose,  you  may  want  to re-use one contained in the
 setup directory. This file essentially contains the compilers
 and librairies with their paths to be used.

 3) Type "make arch=<arch>". This  should create an executable
 in the bin/<arch> directory called xhpl.

 For example, on our Linux PII cluster, I create a file called
 Make.Linux_PII in the top-level directory. Then, I type
    "make arch=Linux_PII"
 This creates the executable file bin/Linux_PII/xhpl.

 4) Quick check: run a few tests:

    cd bin/<arch>
    mpirun -np 4 xhpl

 5) Tuning: Most of the performance  parameters can be tuned,
 by modifying the input file bin/HPL.dat. See the file TUNING
 in the top-level directory.
```

As reported in the above file we have first to create a Make.Something file in the top dir.
We can start by copying one example from the setup directory:

```
  >cp setup/Make.Linux_Intel64 .
```

We now edit such file and change a few things:

1. We define appropriately the TOPDIR variable: `TOPdir       = $(HOME)/hpl`
   We need to set it to the dir where we are, for instance in our case:
   
   ```TOPdir       = $(HOME)/hpl-2.3```
   
2. We comment out the MPI variable becouse we will use the the mpi wrapper compiler (see later)

```
   #MPdir        = /usr/local/mpi
   #MPinc        = -I$(MPdir)/include
   #MPlib        = $(MPdir)/lib/libmpich.a
``` 

3. We define the LAdir variable equal to the MKLROOT environment variable which is defined once we load the appropriate module

    `LAdir        = $(MKLROOT)`

4. We then need to define the LAlib variable. In order to do that we will use the Intel online tool available at  https://software.intel.com/en-us/articles/intel-mkl-link-line-advisor. 
 Once identfied correctly all the options required we then copy&paste the line generated as LAlib value.
 Here below a possibile solution for a Linux O.S. with a multithread library using a gnu compiler is reported:
 
``` 
 LAlib        =  -L${MKLROOT}/lib/intel64 -lmkl_rt -lpthread -lm -ldl
```

5. We finally define the compiler and the linker replacing the original value with the mpicc wrapper made available by the mpi module once loaded.
 

```
CC           = mpicc
CCNOOPT      = $(HPL_DEFS)
CCFLAGS      = $(HPL_DEFS) -fomit-frame-pointer -O3 -funroll-loops -W -Wall
#
# On some platforms,  it is necessary  to use the Fortran linker to find
# the Fortran internals used in the BLAS library.
#
LINKER       = mpicc
```

Done this we can start compiling after loading the appropriate modules: 
For the ORFEO cluster we need to do the following: 

```
 module load intel/20.1 
 make arch=Linux_Intel64 
```

And finally we will find the executable in ` TOPDIR/bin/Linux_Intel64/` 
We are now ready to play with it.
    

## Things  to do:

1.Get a GPU node and run 48 core as 48 mpi processes the xhpl program and finds out the right combination
in term of N(size of the problem)  Nb (block size)  P and Q  (grid of the MPI processes) 
that allows you to get at least 75% of peak performance..
For an initial guess of them here a simple online calculator:

http://www.advancedclustering.com/act_kb/tune-hpl-dat-file/

The input file goes this way:

```
This two lines are 
just for comments
HPL.out      output file name (if any)
6            device out (6=stdout,7=stderr,file)
1            # of problems sizes (N)    #<<<<<<<<<<<<<<<<<<<<
16384        Ns                         #<<<<<<<<<<<<<<<<<<<<
1            # of NBs                   #<<<<<<<<<<<<<<<<<<<<
176          NBs                        #<<<<<<<<<<<<<<<<<<<<
0            PMAP process mapping (0=Row-,1=Column-major)
1            # of process grids (P x Q) #<<<<<<<<<<<<<<<<<<<<
6            Ps                         #<<<<<<<<<<<<<<<<<<<<
4            Qs                         #<<<<<<<<<<<<<<<<<<<<
16.0         threshold
1            # of panel fact
0 1 2        PFACTs (0=left, 1=Crout, 2=Right)
1            # of recursive stopping criterium
2 8          NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
1            # of recursive panel fact.
0 1 2        RFACTs (0=left, 1=Crout, 2=Right)
1            # of broadcast
0 2          BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
1            # of lookahead depth
1 0          DEPTHs (>=0)
1            SWAP (0=bin-exch,1=long,2=mix)
192          swapping threshold
1            L1 in (0=transposed,1=no-transposed) form
1            U  in (0=transposed,1=no-transposed) form
1            Equilibration (0=no,1=yes)
8            memory alignment in double (> 0)
```


2. Try to run the highly optimized version of HPL which Intel provides precompiled: 

You should find it in the following location: 

```
  $MKLROOT/benchmarks/linpack
```

Once identify the executable try to run it with the best combination you obtained in point 1. 

3. Compare and comment performance you obtained with the two different executable

4.Prepare a set of different HPL.dat file to run the hpl benchmark compiled by you using the following combinations; for each case remember to set correctly the  `OMP_NUM_THREADS` variable

  MPI processes | Threads
  --- | --- 
 48 | 1
 24 | 2 
 12 | 4 
 6  | 8 
 2  | 24
 1  | 48 

5.Collect numbers you obtained, plot (if needed) them and prepare a short report comparing peak performance against the performance you obtained. 

6.(optional): Try to run the benchmark using more than one nodes (i.e. 96 cores on two nodes) and see which kind of performance are you able to obtain. Comment the result you obtained..

  Hints: on a larger number of processors you have to enlarge N (size of the problem) and P&Q  while you may keep Nb costant...
