## STREAM benchmark

The STREAM benchmark is a simple synthetic benchmark program that measures sustainable memory bandwidth (in MB/s) and the corresponding computation rate for simple vector kernels of size N.

For the benchmark to work correctly you need to [adjust the problem size](http://www.cs.virginia.edu/stream/ref.html#size), N, so that the data is not cacheable. In this way you are truly measuring memory bandwidth.

### Resources

- https://www.cs.virginia.edu/stream/ref.html
- https://community.intel.com/t5/Intel-C-Compiler/STREAM-OMP-benchmark-compiled-with-ICC/td-p/1046864
- https://blogs.fau.de/hager/archives/8263

-----

### 1 Compile STREAM 

 1. Download STREAM

    `git clone https://github.com/jeffhammond/STREAM.git `

 2. Adjust the problem size to be at least 4 times the size of L3 caches. For this you need to edit the source code `stream.c`, specifically the following line:

    ```c
    #ifndef STREAM_ARRAY_SIZE
    #   define STREAM_ARRAY_SIZE    10000000
    #endif
    ```

    Playing with STREAM_ARRAY_SIZE in the order of 80 millions should be more than enough on our system.

    **Tip:**  you can check the L3 cache size using the command `lscpu`

 3. To compile run the following commands:

    ```
    module load gnu
    cd STREAM
    make all
    ```

    

### 2. Measure memory bandwidth

**2.1** STREAM is a really fast benchmark so to run it you can simply ask a job in interactive mode:

`srun -N1 --exclusive -p THIN --time=1:0:0 --mem=50G --pty bash`

To measure the full node memory bandwidth you need ask for all 24 processors (on a thin node).

To run the test simply run the executable:

`./stream_c.exe`

**2.2** the naive way 

We can now run the Intel code compiled using openMP as it is:

``` 
[cozzini@ct1pt-tnode006 stream]$ ./stream_intel_omp.x
-------------------------------------------------------------
STREAM version $Revision: 5.10 $
-------------------------------------------------------------
This system uses 8 bytes per array element.
-------------------------------------------------------------
Array size = 88000000 (elements), Offset = 0 (elements)
Memory per array = 671.4 MiB (= 0.7 GiB).
Total memory required = 2014.2 MiB (= 2.0 GiB).
Each kernel will be executed 10 times.
 The *best* time for each kernel (excluding the first iteration)
...
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:           86972.1     0.017072     0.016189     0.020788
Scale:          87095.2     0.016653     0.016166     0.019954
Add:            99201.2     0.022018     0.021290     0.027231
Triad:          98645.5     0.022046     0.021410     0.026984

```

Numbers are far from peak-performance of the thin node (128Gb/sec on each CPU for a total of 256GB()
Moreover repeting the test they are not at all stable with large fluctuation.
Reasons of such behaviour are based on the thread affinity problem as clearly stated in all references at the end of this document.


**2.2** the right way  

We therefore  need to pin our threads on cores in order to get both stability and performance;
We use in this case likwid-pin utily but other choices are also available.

Numbers are now stable and increases considerably as well:

```
[cozzini@ct1pt-tnode006 stream]$ likwid-pin -c N:0-23 ./stream_intel_omp.x
-------------------------------------------------------------
STREAM version $Revision: 5.10 $
-------------------------------------------------------------
This system uses 8 bytes per array element.
-------------------------------------------------------------
Array size = 88000000 (elements), Offset = 0 (elements)
Memory per array = 671.4 MiB (= 0.7 GiB).
Total memory required = 2014.2 MiB (= 2.0 GiB).
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:          160364.4     0.008870     0.008780     0.009004
Scale:         159313.2     0.009750     0.008838     0.015147
Add:           166651.7     0.013455     0.012673     0.019113
Triad:         166639.1     0.012754     0.012674     0.012905
-------------------------------------------------------------
```

Performance are now in the order of 64% of peak performance.


1. Try it out using 12 and 24 OMP threads (reminder: `export OMP_NUM_THREADS=12`)

2. Compare the results against the theoretical memory bandwidth which for a thin node is: 128GB/sec on each socket for a total of 256 GB/sec.

**2.2 Understanding the cost of wrong allocation**

Using the previous results as a baseline play with different binding schemes to appreciate performance difference.

To do this you can use `numactl` (there are may other ways of doing this):

```_c
 numactl --cpunodebind 0 --membind 0 ./stream_c.exe
```

Benchmark the memory bandwidth as you increase the number of threads from 1 to 12 using the following configuration:

- threads and memory allocated in the same socket (cpunode= 0, m= 0)
- threads and memory allocated in different socket (cpunode= 0, m= 1)

Plot and comment the results.




