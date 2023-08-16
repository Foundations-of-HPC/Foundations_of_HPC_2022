#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --time=2:0:0
#SBATCH -n 128
#SBATCH -N1

echo "Loading modules"
module load mkl
module load openBLAS/0.3.23-omp

OPENBLASROOT=${OPENBLAS_ROOT}

rm mkl*output.txt 
rm counter.txt
echo "Running executables..."
output=$(srun -n1 --cpus-per-task=64 ./gemm_mkl.x)

echo "$output" | grep "Currently" >> counter.txt
echo "$output" | grep "GFLOPS" >> mkl-output.txt

echo "Finished."
