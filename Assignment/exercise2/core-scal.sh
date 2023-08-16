#!/bin/bash

echo "Loading modules"
module load mkl
module load openBLAS/0.3.23-omp

OPENBLASROOT=${OPENBLAS_ROOT}

echo "Removing files ending with /"output.txt/""
rm *output.txt
echo "Running executables..."

for i in {1..128}
do{
	echo "Currently on $i cores"
	output=$(srun -n1 --cpus-per-task=$i ./gemm_mkl.x)
	echo "$output" | grep "GFLOPS" >> mkl-core-output.txt

	output=$(srun -n1 --cpus-per-task=$i ./gemm_oblas.x)
	echo "$output" | grep "GFLOPS" >> oblas-core-output.txt
}
done

echo "Finished."

