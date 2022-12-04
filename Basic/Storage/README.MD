## I/O storage for HPC and parallel FS  

## OUTLINE

This set of lecture gives a short overview of HPC storage system and associated concepts. The associated case study is the ORFEO storage system.
Benchmarks concepts are also presented and applied specifically to the I/O storage system of ORFEO.


## SLIDES

- [Storage for HPC](lectures-Storage-for-HPC.pdf)

## References&Books

- Mainly Chapter 17 and Chapter 18 of Sterling's book  
- http://www.prace-ri.eu/best-practice-guide-parallel-i-o/#id-1.3.5
- http://wiki.lustre.org/Main_Page and in particular the pdf below:
- http://wiki.lustre.org/images/6/64/LustreArchitecture-v4.pdf
- [a web site with a lot of useful information on I/O for HPC](https://www.vi4io.org/start)

## practical_sessions 

- Identify your laptop FS and associated metadata
- [compile and run iozone](install_iozone.md)
- measure I/O performance of your laptop by means of iozone
- measure I/O performance on ORFEO filesystem by means of iozone
- [compile and run IOR + mdtest on different ORFEO fs](install_IOR.md)
