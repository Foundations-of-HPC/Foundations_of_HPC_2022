#!/bin/bash
#SBATCH -A dssc
#SBATCH --partition=EPYC
#SBATCH --time=01:0:0
#SBATCH -n 64
#SBATCH -N1

echo "mkl matrix scaling"
echo "********************************"
echo "Loading modules"
module load mkl

output_file="./outputs/matrix_scaling/mkl-matrx-scaling-$SLURM_JOB_ID.csv"

# Add header
echo "Matrix Size (n),Seconds,GFLOPS,Precision"> "$output_file"

# Single precision scaling
for((i=2000; i <= 20000; i+=500))
do
	echo "Iteration $i"

	mkl_out=$(srun -n1 --cpus-per-task=64 ./gemm_mkl_single.x $i $i $i)
	
	# Extract the relevant information (seconds and GFLOPS)
    seconds=$(echo "$mkl_out" | tail -n 1 | awk '{print $2}')
    gflops=$(echo "$mkl_out" | tail -n 1 | awk '{print $4}')

	echo "$i, $seconds, $gflops,Single" >> "$output_file"

	mkl_out=$(srun -n1 --cpus-per-task=64 ./gemm_mkl_double.x $i $i $i)
	
	# Extract the relevant information (seconds and GFLOPS)
    seconds=$(echo "$mkl_out" | tail -n 1 | awk '{print $2}')
    gflops=$(echo "$mkl_out" | tail -n 1 | awk '{print $4}')

	echo "$i, $seconds, $gflops,Double" >> "$output_file"

done

#
echo "Finished"
