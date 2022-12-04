# How to install and run [ior](https://github.com/hpc/ior/releases) on ORFEO storage systems  

First of all, let us request a epyc node and load the openMPI module 
```console

$ salloc -n4 -N1 -t 0:45:00 -p FHPC
salloc: Granted job allocation 545
$ module load architecture/AMD
$ module load openMPI/4.1.4/

```

Get the most recent version of `ior` from its [GitHub releases](https://github.com/hpc/ior/releases).
At the time of this writing this is version 3.3.0 

```console
$ 
$ wget https://github.com/hpc/ior/releases/download/3.3.0/ior-3.3.0.tar.gz
$ tar -xf ior-3.3.0.tar.gz && cd ior-3.3.0/
```
 
Configure and build it; version `3.3.0` provides support for CEPH filesystem and so we would like to test it.
In order to enable it we will pass ``with-cephfs`` option to the configure together with the `` with--mpiio`` one to enable the MPIIO interface.
 
```console
$ ./configure --prefix=$HOME/software/ior/${VER}/ CC=$(which mpicc)  --with-mpiio --with-cephfs

Once configuration has been completed we have to modify just one line in the file `` src/aiori-CEPHFS.c`` 
Edit the file and line 17 should be changed from `` #ifdef HAVE_CONFIG_H `` to `` #ifndef HAVE_CONFIG_H ``

Once modified this file we can compile and install the code 

$ make -j4
$ make install
```
At this point the ``ior `` and 	``mdtest`` program are ready to used for testing.


## Testing metadata 

To perform a benchmark with mdtest:
```console
$ cd $HOME/software/ior/${VER}/bin/
$ srun $PWD/mdtest -d <filesystem> -n 10 -i 200 -y
```
where <filesystem> can be `/fast`, `/scratch ` (both CEPH FS but with different configuration,  a local directory (`/tmp`) and the NFS filesystem (`//TEST_onp_run`) 
The options means that mdtest will perform 200 iteration (`-i 200`) of the test creating each time 10 files ('-n 10') and synchronazing ('-y') at each iteration

To get more options for mdtest, read the help: `mdtest -h`.

### proposed exercise:

 -run on the fourth different filesystems and report/discuss differences in performance.


## Getting Started with IOR

This section is taken and slightly modified from the original IOR documentation.


IOR writes data sequentially with the following parameters:

   * ``blockSize`` (``-b``)
   * ``transferSize`` (``-t``)
   * ``segmentCount`` (``-s``)
   * ``numTasks`` (``-n``)

which are best illustrated with a diagram:

.. image:: tutorial-ior-io-pattern.png


These four parameters are all you need to get started with IOR.  However, naively running IOR usually gives disappointing results.
Let us select for instance the ``/fast`` cephs and run a 24mpi processes asking to write 16 segment of 16 Mb each with a transfer size of 1 Mb.

 For example, if we run
a four-node IOR test that writes a total of 16 GiB:



```

$ mpirun -np 24 ./ior  -t 1m -b 16m -s 24  -o /fast/area/cozzini/test
....
Results:

access    bw(MiB/s)  IOPS       Latency(s)  block(KiB) xfer(KiB)  open(s)    wr/rd(s)   close(s)   total(s)   iter
------    ---------  ----       ----------  ---------- ---------  --------   --------   --------   --------   ----
write     251.95     251.96     2.29        16384      1024.00    0.002620   36.58      10.31      36.58      0
read      142.43     142.43     4.00        16384      1024.00    0.000577   64.70      39.32      64.70      0
remove    -          -          -           -          -          -          -          -          0.000944   0
Max Write: 251.95 MiB/sec (264.19 MB/sec)
Max Read:  142.43 MiB/sec (149.35 MB/sec)


```

As you can see we get quite poor performance out of CEPHfs parallel file
system that should be capable of a lot more.

Switching from writing to a single-shared file to one file per process using the
``-F`` (``filePerProcess=1``) option changes the performance dramatically:

``` 

$ mpirun -np 24 ./ior  -t 1m -b 16m -s 24  -o /fast/area/cozzini/test
...
$ mpirun -np 24 ./ior  -t 1m -b 16m -s 24  -o /fast/area/cozzini/test
Results:

access    bw(MiB/s)  IOPS       Latency(s)  block(KiB) xfer(KiB)  open(s)    wr/rd(s)   close(s)   total(s)   iter
------    ---------  ----       ----------  ---------- ---------  --------   --------   --------   --------   ----
write     26686      26727      0.020660    16384      1024.00    0.007102   0.344825   0.095334   0.345344   0
read      239576     239818     0.002367    16384      1024.00    0.000148   0.038429   0.006301   0.038468   0
remove    -          -          -           -          -          -          -          -          0.004600   0
Max Write: 26686.41 MiB/sec (27982.73 MB/sec)
Max Read:  239575.53 MiB/sec (251213.15 MB/sec)
```

This is in large part because letting each MPI process work on its own file cuts out any contention that would arise because of file locking.

However, the performance difference between our naive test and the
file-per-process test is a bit extreme.  In fact, the only way that 251 GB/sec
read rate could be achievable on CEPH is if each of the four storage node dedcated to ``/fast`` is able to provide a network bandwidth of about 60GB/sec ! 
This is of course not possible.


Effect of Page Cache on Benchmarking
------------------------------------
What's really happening is that the data being read by IOR isn't actually coming
from ``/fast``; rather, files' contents are already cached, and IOR is able to
read them directly out of each compute node's DRAM.  The data wound up getting
cached during the write phase of IOR as a result of Linux (and CEPH as well) using a
write-back cache to buffer I/O, so that instead of IOR writing and reading data
directly to ``/fast``  it's actually mostly talking to the memory on each compute
node.

To be more specific, although each IOR process thinks it is writing to a file on
/fast and then reading back the contents of that file from /fast, it is
actually

    1)  writing data to a copy of the file that is cached in memory.  If there
        is no copy of the file cached in memory before this write, the parts
        being modified are loaded into memory first.
    2)  those parts of the file in memory (called "pages") that are now
        different from what's on /fast are marked as being "dirty"
    3)  the write() call completes and IOR continues on, even though the written
        data still hasn't been committed to /fast
    4)  independent of IOR, the OS kernel continually scans the file cache for
        files who have been updated in memory but not on /fast ("dirt pages"),
        and then commits the cached modifications to Lustre
    5)  dirty pages are declared non-dirty since they are now in sync with
        what's on disk, but they remain in memory

Then when the read phase of IOR follows the write phase, IOR is able to just
retrieve the file's contents from memory instead of having to communicate with
/fast over the network.

There are a couple of ways to measure the read performance of the underlying
fast file system. The most crude way is to simply write more data than will
fit into the total page cache so that by the time the write phase has completed,
the beginning of the file has already been evicted from cache. For example,
increasing the number of segments (``-s``) to write more data reveals the point at
which the nodes' page cache on my test system runs over very clearly:

.. image:: tutorial-ior-overflowing-cache.png


However, this can make running IOR on systems with a lot of on-node memory take
forever.

A better option would be to get the MPI processes on each node to only read data
that they didn't write.  For example, on a four-process-per-node test, shifting
the mapping of MPI processes to blocks by four makes each node N read the data
written by node N-1.

.. image:: tutorial-ior-reorderTasks.png

Since page cache is not shared between compute nodes, shifting tasks this way
ensures that each MPI process is reading data it did not write.

IOR provides the ``-C`` option (``reorderTasks``) to do this, and it forces each MPI
process to read the data written by its neighboring node. Running IOR with
this option gives much more credible read performance:

```

$ mpirun -np 24 ./ior  -t 1m -b 16m -s 24 -F -C -o /fast/area/cozzini/test

Results:

access    bw(MiB/s)  IOPS       Latency(s)  block(KiB) xfer(KiB)  open(s)    wr/rd(s)   close(s)   total(s)   iter
------    ---------  ----       ----------  ---------- ---------  --------   --------   --------   --------   ----
write     25627      25661      0.020860    16384      1024.00    0.007562   0.359145   0.112631   0.359625   0

read      840.71     840.71     0.678779    16384      1024.00    0.303704   10.96      10.93      10.96      0
remove    -          -          -           -          -          -          -          -          0.009650   0
Max Write: 25626.72 MiB/sec (26871.57 MB/sec)
Max Read:  840.71 MiB/sec (881.55 MB/sec)

```


But now it should seem obvious that the write performance is also ridiculously
high. And again, this is due to the page cache, which signals to IOR that writes
are complete when they have been committed to memory rather than the underlying
/fast file system.

To work around the effects of the page cache on write performance, we can issue
an fsync() call immediately after all of the write()s return to force the dirty
pages we just wrote to flush out to /fast. Including the time it takes for
fsync() to finish gives us a measure of how long it takes for our data to write
to the page cache and for the page cache to write back to Lustre.

IOR provides another convenient option, ``-e`` (fsync), to do just this. And, once
again, using this option changes our performance measurement quite a bit:

``` 

Results:

access    bw(MiB/s)  IOPS       Latency(s)  block(KiB) xfer(KiB)  open(s)    wr/rd(s)   close(s)   total(s)   iter
------    ---------  ----       ----------  ---------- ---------  --------   --------   --------   --------   ----
write     940.44     940.50     0.604849    16384      1024.00    0.006298   9.80       8.99       9.80       0
read      2489.21    2489.26    0.198068    16384      1024.00    0.419163   3.70       3.67       3.70       0
remove    -          -          -           -          -          -          -          -          0.009644   0
Max Write: 940.44 MiB/sec (986.13 MB/sec)
Max Read:  2489.21 MiB/sec (2610.13 MB/sec)

and we finally have a believable bandwidth measurement for our file system.
We notice however that now the READ performance is larger than the Write one.
Overall performance are globally not so satisfactory.


Defeating Page Cache
--------------------
Since IOR is specifically designed to benchmark I/O, it provides these options
that make it as easy as possible to ensure that you are actually measuring the
performance of your file system and not your compute nodes' memory.  That being
said, the I/O patterns it generates are designed to demonstrate peak performance,
not reflect what a real application might be trying to do, and as a result,
there are plenty of cases where measuring I/O performance with IOR is not always
the best choice.  There are several ways in which we can get clever and defeat
page cache in a more general sense to get meaningful performance numbers.

When measuring write performance, bypassing page cache is actually quite simple;
opening a file with the ``O_DIRECT`` flag going directly to disk.  In addition,
the ``fsync()`` call can be inserted into applications, as is done with IOR's ``-e``
option.

Measuring read performance is a lot trickier.  If you are fortunate enough to
have root access on a test system, you can force the Linux kernel to empty out
its page cache by doing

.. code-block:: shell

   # echo 1 > /proc/sys/vm/drop_caches

and in fact, this is often good practice before running any benchmark
(e.g., Linpack) because it ensures that you aren't losing performance to the
kernel trying to evict pages as your benchmark application starts allocating
memory for its own use.

Unfortunately, many of us do not have root on our systems, so we have to get
even more clever.  As it turns out, there is a way to pass a hint to the kernel
that a file is no longer needed in page cache:

.. code-block:: c

   #define _XOPEN_SOURCE 600
   #include <unistd.h>
   #include <fcntl.h>
   int main(int argc, char *argv[]) {
       int fd;
       fd = open(argv[1], O_RDONLY);
       fdatasync(fd);
       posix_fadvise(fd, 0,0,POSIX_FADV_DONTNEED);
       close(fd);
       return 0;
   }

The effect of passing POSIX_FADV_DONTNEED using ``posix_fadvise()`` is usually that
all pages belonging to that file are evicted from page cache in Linux.  However,
this is just a hint --not a guarantee-- and the kernel evicts these pages
asynchronously, so it may take a second or two for pages to actually leave page
cache.  Fortunately, Linux also provides a way to probe pages in a file to see
if they are resident in memory.

Finally, it's often easiest to just limit the amount of memory available for
page cache.  Because application memory always takes precedence over cache
memory, simply allocating most of the memory on a node will force most of the
cached pages to be evicted.  Newer versions of IOR provide the memoryPerNode
option that do just that, and the effects are what one would expect:

.. image:: tutorial-ior-memPerNode-test.png

The above diagram shows the measured bandwidth from a single node with 128 GiB
of total DRAM.  The first percent on each x-label is the amount of this 128 GiB
that was reserved by the benchmark as application memory, and the second percent
is the total write volume.  For example, the "50%/150%" data points correspond
to 50% of the node memory (64 GiB) being allocated for the application, and a
total of 192 GiB of data being read.

This benchmark was run on a single spinning disk which is not capable of more
than 130 MB/sec, so the conditions that showed performance higher than this were
benefiting from some pages being served from cache.  And this makes perfect
sense given that the anomalously high performance measurements were obtained
when there was plenty of memory to cache relative to the amount of data being
read.

Corollary
---------
Measuring I/O performance is a bit trickier than CPU performance in large part
due to the effects of page caching.  That being said, page cache exists for a
reason, and there are many cases where an application's I/O performance really
is best represented by a benchmark that heavily utilizes cache.

For example, the BLAST bioinformatics application re-reads all of its input data
twice; the first time initializes data structures, and the second time fills
them up.  Because the first read caches each page and allows the second read to
come out of cache rather than the file system, running this I/O pattern with
page cache disabled causes it to be about 2x slower:


