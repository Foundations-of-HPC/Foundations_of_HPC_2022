## Stream benchmark 

In this tutorial we play with the STREAM benchmark to measure memory bandwidth on the HPC multicore nodes.
A few operation will be performed:
  - measure the overall bandwidth on one node and compare with expected peak-performance
  - discuss the role of thread affinity 
  - check the penalties you may pay in case of wrong memory allocation

### compilation 

 We start first compiling the benchmark that is composed by a sigle source code.
 We dowmnload it:

`` wget  ``

and we then compile for both Intel and Gnu compilers using the following set of optimazation flags:


``  make all ``

the benchmark allows to pass some flags to chage the size of the array, that should be at least 4 times the size of L3 caches.
Playing with N in the order of 80 millions should be more than enough on our systems.


### section one: measure bandwidth

#### the naive way 

We can run the Intel code compiled using openMP as it is:

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


We therefore pin our threads on cores and performance start to be the same  and considerably increase;
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


#### section two: understand the cost of wrong allocation

In this section we will execute STREAM on just the cores of on single socket. 
With the following line we loop over 12 threads. 
We use `numactl` to bind the process to socket `0` and local memory `0`.

```
(for i in `seq 1 12`; do OMP_NUM_THREADS=$i numactl --cpunodebind 0 --membind 0 ./stream; done) | grep "Triad:" | awk '{print $2}'
```

Now we force the process to use distant memory, simply correcting `--membind 1`.


```
(for i in `seq 1 12`; do OMP_NUM_THREADS=$i numactl --cpunodebind 0 --membind 1 ./stream; done) | grep "Triad:" | awk '{print $2}'
```

Performance numbers can be different.

#### proposed exercise 

 - create a plot for ORFEO nodes and check if the difference in performance is simmetric: i.e. bind core on socket 0 and memory on socket 1 you obtain the same performance as binding cores on socket 1 and memory on socket 0. 

 -play with different binding schema to appreciate performance difference


### References

 - https://www.cs.virginia.edu/stream/ref.html
 - https://community.intel.com/t5/Intel-C-Compiler/STREAM-OMP-benchmark-compiled-with-ICC/td-p/1046864
 - https://blogs.fau.de/hager/archives/8263

