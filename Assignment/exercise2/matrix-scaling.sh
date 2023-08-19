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


for((i=2000; i <= 20000; i+=500))
do
	mkl_out=$(srun -n1 --cpus-per-task=64 ./gemm_mkl.x $i $i $i)
	oblas_out=$(srun -n1 --cpus-per-task=64 ./gemm_oblas.x $i $i $i)

	echo "$mkl_out" | tail -n 1 >> /outputs/mkl-matrx-scaling-$SLURM_JOB_ID.txt
	echo "$oblas_out" | tail -n 1 >> /outputs/oblas-matrx-scaling-$SLURM_JOB_ID.txt
done

echo "Finished"
