#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=piero_job
#SBATCH --time=1:0:0
#SBATCH -n 128
#SBATCH -N1

echo $@
/bin/hostname
srun ./gemm_mkl.x "$@" | tail -n 1 >> mkl-output.txt

srun ./gemm_oblas.x "$@"  | tail -n 1 >> oblas-output.txt

rm slurm*
