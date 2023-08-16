### MKL libraries
###
###
MKL= -L${MKLROOT}/lib/intel64  -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl

### OpenBLAS libraries 
OPENBLASROOT=${OPENBLAS_ROOT}
### BLIS library
BLISROOT=/u/dssc/ppette00/myblis/lib
PRECISION = 1 

ifeq ($(PRECISION),1)
	PREC_FLAG = -DUSE_FLOAT
else
	PREC_FLAG = -DUSE_DOUBLE
endif

cpu: gemm_mkl.x gemm_oblas.x gemm_blis.x

gemm_mkl.x: dgemm.c
	gcc $(PREC_FLAG) -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

gemm_oblas.x: dgemm.c
	gcc $(PREC_FLAG) -DOPENBLAS $^ -m64 -I${OPENBLASROOT}/include -L/${OPENBLASROOT}/lib -lopenblas -lpthread -o $@ -fopenmp

gemm_blis.x: gemm.c
	gcc $(PREC_FLAG) -DBLIS $^ -m64 -I${BLISROOT}/include/blis -L/${BLISROOT}/lib -o $@ -lpthread  -lblis -fopenmp -lm
clean:
	rm -rf *.x
