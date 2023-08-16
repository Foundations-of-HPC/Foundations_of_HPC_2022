#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=piero_job
#SBATCH --time=1:0:0
#SBATCH -n 128
#SBATCH -N1


rm *output.txt
while IFS= read -r line; do
	./gemm_mkl.x $line | tail -n 1 >> mkl-output.txt
	echo $line
	./gemm_oblas.x $line | tail -n 1 >> oblas-output.txt
done < arguments.txt
