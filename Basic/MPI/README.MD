## MPI section 

A collection of materials/references for the MPI lectures 


### lectures (all by S.Cozzini)
 
 - lecture 4: [introduction to parallel computing]()
 - lecture 5: [Message Passing and MPI programming](lecture05-MessagePassing-and-MPI-Programming.pdf)
 - lecture 6: [Collective operation and virtual topology](lecture06-Collective-MPI-virtual-topology.pdf)
 - lecture 7: [domain decomposition with MPI](lecture07-Domain-Decomposition-with-MPI.pdf)
 - lecture 8: [Network basic for MPI applications](lecture08-Network-basics-for-MPI-application.pdf)
 - lecture 9: [MPI libraries and performance ](lecture09-mpi-libraries-and-performance.pdf)
 - lecture 10:[Performance model for MPI applications](lecture10-PerformanceEvaluationforMPI-applications.pdf)
 


### Main references for MPI lectures: 
 - chapter 9 of reference 4 is a nice and detailed introduction to MPI. The final section of the chapter is lecture 7 
 - exercises and tutorials on  MPI are present all over the web. Here a couple of examples:
    - [Here a very good starting point]https://www.mcs.anl.gov/research/projects/mpi/tutorial/index.html)
    - [Another simple tutorial](https://mpitutorial.com/tutorials/)
    - 

###  notes on the codes provided:


#### Jacoby code
The  code here provided is taken from the [ website of reference 4 book](https://blogs.fau.de/hager/hpc-book#teaching)
It is a "simple" fortran implementation of the Jacobi solver in 3D

To compile, load the MPI module and then:

 ` mpif77 -ffixed-line-length-none Jacobi_MPI_vectormode.F -o jacoby3D.x `

To run the code

` mpirun --mca btl ^openib -np 24 ./jacoby3D.x  `

#### Installing and running MPI intel benchmark

get the benchmark:
 
 ` git pull https://github.com/intel/mpi-benchmarks`

Enter in the dir: 
 
 `  cd mpi-benchmarks/src_c `

Compile the code, once you load the module: 
 
 ` make `

Run the executable across two nodes: 

 ` mpirun -np 2  --map-by node --report-bindings  ./IMB-MPI1 PingPong `

Run the executable across two socket: 
 
 ` mpirun -np 2 --report-bindings --map-by socket ./IMB-MPI1 PingPong `
 
Run the executable within the same socket:

   ` mpirun -np 2 --report-bindings --map-by socket ./IMB-MPI1 PingPong `

Run the executable using a different protocol: tcp and not infiniband

   ` mpirun -np 2 --mca pml ob1 --mca btl tcp,self --mca btl_tcp_if_include br0 ./IMB-MPI1 PingPong `

  

