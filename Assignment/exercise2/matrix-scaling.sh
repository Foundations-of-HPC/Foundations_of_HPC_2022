#!/bin/bash
#SBATCH -A dssc
#SBATCH --partition=EPYC
#SBATCH --time=01:0:0
#SBATCH -n 64
#SBATCH -N1

echo "Loading modules"
module load mkl
module load openBLAS/0.3.23-omp

OPENBLASROOT=${OPENBLAS_ROOT}
export LD_LIBRARY_PATH=/u/dssc/ppette00/myblis/lib:$LD_LIBRARY_PATH

# Single precision scaling
for((i=2000; i <= 20000; i+=500))
do
	echo "Iteration $i"

	mkl_out=$(srun -n1 --cpus-per-task=64 ./gemm_mkl_single.x $i $i $i)
	oblas_out=$(srun -n1 --cpus-per-task=64 ./gemm_oblas_single.x $i $i $i)
	blis_out=$(srun -n1 --cpus-per-task=64 ./gemm_blis_single.x $i $i $i)

	echo "$mkl_out" | tail -n 1 >> ./outputs/matrix_scaling/mkl-matrx-scaling-single-$SLURM_JOB_ID.txt
	echo "$oblas_out" | tail -n 1 >> ./outputs/matrix_scaling/oblas-matrx-scaling-single-$SLURM_JOB_ID.txt
	echo "$blis_out" | tail -n 1 >> ./outputs/matrix_scaling/blis-matrx-scaling-single-$SLURM_JOB_ID.txt

	mkl_out=$(srun -n1 --cpus-per-task=64 ./gemm_mkl_double.x $i $i $i)
	oblas_out=$(srun -n1 --cpus-per-task=64 ./gemm_oblas_double.x $i $i $i)
	blis_out=$(srun -n1 --cpus-per-task=64 ./gemm_blis_double.x $i $i $i)

	echo "$mkl_out" | tail -n 1 >> ./outputs/matrix_scaling/mkl-matrx-scaling-double-$SLURM_JOB_ID.txt
	echo "$oblas_out" | tail -n 1 >> ./outputs/matrix_scaling/oblas-matrx-scaling-double-$SLURM_JOB_ID.txt
	echo "$blis_out" | tail -n 1 >> ./outputs/matrix_scaling/blis-matrx-scaling-double-$SLURM_JOB_ID.txt
done

#
echo "Finished"
