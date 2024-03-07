#!/bin/bash
#SBATCH -A dssc
#SBATCH --partition=EPYC
#SBATCH --time=0:30:0
#SBATCH -n 128
#SBATCH -N1

echo "OpenBLAS core scaling"
echo "********************************"
echo "Loading modules"
module load openBLAS/0.3.23-omp

OPENBLASROOT=${OPENBLAS_ROOT}

output_file="./outputs/core_scaling/oblas-core-scaling-$SLURM_JOB_ID.csv"

# Add header
echo "Number of CPUs,Seconds,GFLOPS,Precision"> "$output_file"


for ((i=1;i<=128;i+=1))
do

	echo "Iteration $i"

	output=$(srun -n1 --cpus-per-task=$i ./gemm_oblas_single.x 10000 10000 10000)

	# Extract relevant information
	seconds = $(echo "$output" | tail -n 1 | awk '{print $2}')
	gflops = $(echo "$output" | tail -n 1 | awk '{print $4}')

	echo "$i, $seconds, $gflops,Single" >> "$output_file"

	output=$(srun -n1 --cpus-per-task=$i ./gemm_oblas_double.x 10000 10000 10000)

	# Extract relevant information
	seconds = $(echo "$output" | tail -n 1 | awk '{print $2}')
	gflops = $(echo "$output" | tail -n 1 | awk '{print $4}')

	echo "$i, $seconds, $gflops,Double" >> "$output_file"

done

echo "Finished"

