#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=piero_job
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
while IFS= read -r line; do
	#srun ./gemm_mkl.x $line | tail -n 1 >> mkl-output.txt
	srun -n1 --cpus-per-task=128 ./gemm_mkl.x $line | tail -n 1 >> mkl-output.txt
	echo $line
	srun -n1 --cpus-per-task=128 ./gemm_oblas.x $line | tail -n 1 >> oblas-output.txt
done < arguments.txt

echo "Finished."
