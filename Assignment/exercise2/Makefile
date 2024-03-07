### MKL libraries
###
###
MKL= -L${MKLROOT}/lib/intel64  -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl

### OpenBLAS libraries 
OPENBLASROOT=${OPENBLAS_ROOT}
### BLIS library
BLISROOT=u/dssc/ppette00/myblis

cpu: gemm_mkl_single.x gemm_oblas_single.x gemm_blis_single.x gemm_mkl_double.x gemm_oblas_double.x gemm_blis_double.x


gemm_mkl_single.x: dgemm.c
	gcc -DUSE_FLOAT -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

gemm_mkl_double.x: dgemm.c
	gcc -DUSE_DOUBLE -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

gemm_oblas_single.x: dgemm.c
	gcc -DUSE_FLOAT -DOPENBLAS $^ -m64 -I${OPENBLASROOT}/include -L/${OPENBLASROOT}/lib -lopenblas -lpthread -o $@ -fopenmp

gemm_oblas_double.x: dgemm.c
	gcc -DUSE_DOUBLE -DOPENBLAS $^ -m64 -I${OPENBLASROOT}/include -L/${OPENBLASROOT}/lib -lopenblas -lpthread -o $@ -fopenmp

gemm_blis_single.x: dgemm.c
	gcc -DUSE_FLOAT  -DBLIS $^ -m64 -I${BLISROOT}/include/blis -L/${BLISROOT}/lib -o $@ -lpthread  -lblis -fopenmp -lm

gemm_blis_double.x: dgemm.c
	gcc -DUSE_DOUBLE  -DBLIS $^ -m64 -I${BLISROOT}/include/blis -L/${BLISROOT}/lib -o $@ -lpthread  -lblis -fopenmp -lm

clean:
	rm -rf *.x
