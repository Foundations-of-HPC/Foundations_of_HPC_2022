#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --time=1:0:0
#SBATCH -n 128
#SBATCH -N1

echo "Loading modules"
module load mkl
module load openBLAS/0.3.23-omp

OPENBLASROOT=${OPENBLAS_ROOT}

echo "Removing files ending with /"output.txt/""
rm *output.txt 
echo "Running executables..."
output=$(srun -n1 --cpus-per-task=64 ./gemm_mkl.x)
echo "$output" | grep "GFLOPS" >> mkl-output.txt


output=$(srun -n1 --cpus-per-task=64 ./gemm_oblas.x) 
echo "$output" | grep "GFLOPS" >> oblas-output.txt


echo "Finished."