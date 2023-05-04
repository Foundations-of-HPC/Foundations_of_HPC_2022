# information about exercise 2

## Build BLIS library 


AMD provide its own implementation of standard BLAS routine, this implementation is provided in the `BLIS` library, <a href="https://developer.amd.com/amd-aocl/blas-library/"> available here </a>.
There is also the source code on github.

Download it:
```
$git clone https://github.com/flame/blis.git
$cd blis
```

Configure and compile with openMP support (multithreading is disabled by default,remember to modify *prefix* path):
```
 srun -n1 ./configure --enable-cblas --enable-threading=openmp --prefix=/u/area/ntosato/myblis auto
 srun  -n 1 --cpus-per-task=128 make -j 128
 make install
```

We compile in the target machine, and we allow the command to use 128 cores, then use `-j 128` argument to compile in parallel way.

To use BLIS with multiple threads:
`export BLIS_NUM_THREADS=128`.

The final artifact will be placed in `/u/area/ntosato/myblis/lib` directory, this is the path that you need to put inside `Makefile` and library path .

To compile the previous exercise with the new BLIS library modify the `Makefile` uncommenting the `BLIS` related rows. 

And adjust LD_LIBRARY_PATH (**modify it with your own path**):
```
 export LD_LIBRARY_PATH=/u/area/ntosato/myblis/lib:$LD_LIBRARY_PATH
```
