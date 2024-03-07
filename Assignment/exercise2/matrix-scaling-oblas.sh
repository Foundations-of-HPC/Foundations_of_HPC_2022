#!/bin/bash
#SBATCH -A dssc
#SBATCH --partition=EPYC
#SBATCH --time=01:0:0
#SBATCH -n 64
#SBATCH -N1

echo "OpenBLAS matrix scaling"
echo "********************************"
echo "Loading modules"
module load openBLAS/0.3.23-omp
OPENBLASROOT=${OPENBLAS_ROOT}

output_file="./outputs/matrix_scaling/oblas-matrx-scaling-$SLURM_JOB_ID.csv"

# Add header
echo "Matrix Size (n),Seconds,GFLOPS,Precision"> "$output_file"

# Single precision scaling
for((i=2000; i <= 20000; i+=500))
do
	echo "Iteration $i"

	out=$(srun -n1 --cpus-per-task=64 ./gemm_oblas_single.x $i $i $i)
	
	# Extract the relevant information (seconds and GFLOPS)
    seconds=$(echo "$out" | tail -n 1 | awk '{print $2}')
    gflops=$(echo "$out" | tail -n 1 | awk '{print $4}')

	echo "$i, $seconds, $gflops,Single" >> "$output_file"

	out=$(srun -n1 --cpus-per-task=64 ./gemm_oblas_double.x $i $i $i)
	
	# Extract the relevant information (seconds and GFLOPS)
    seconds=$(echo "$out" | tail -n 1 | awk '{print $2}')
    gflops=$(echo "$out" | tail -n 1 | awk '{print $4}')

	echo "$i, $seconds, $gflops,Double" >> "$output_file"

done

#
echo "Finished"
