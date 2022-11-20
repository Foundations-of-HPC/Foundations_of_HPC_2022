PROGRAM hello
 INCLUDE ‘mpif.h‘
 INTEGER err
 CALL MPI_INIT(err)
 call MPI_COMM_RANK(MPI_COMM_WORLD,rank,ierr)
 call MPI_COMM_SIZE(MPI_COMM_WORLD,size,ierr)
 print *, 'I am ', rank, ' of ', size
 CALL MPI_FINALIZE(err)
END
