#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --time=01:0:0
#SBATCH -n 128
#SBATCH -N1
#SBATCH --output=/outputs/slurm-%j.out

echo "Loading modules"
module load mkl
module load openBLAS/0.3.23-omp

OPENBLASROOT=${OPENBLAS_ROOT}

for ((i=1;i<=128;i+=1)) 
do
	mkl_out=$(srun -n1 --cpus-per-task=$i ./gemm_mkl.x 10000 10000 10000)
	oblas_out=$(srun -n1 --cpus-per-task=$i ./gemm_oblas.x 10000 10000 10000)

	echo "$mkl_out      $i" | tail -n 1 >> /outputs/mkl-core-scaling-$SLURM_JOB_ID.txt
	echo "$oblas_out      $i" | tail -n 1 >> /outputs/oblas-core-scaling-$SLURM_JOB_ID.txt
done

echo "Finished"
