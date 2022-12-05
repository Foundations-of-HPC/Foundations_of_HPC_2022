#!/bin/bash
#SBATCH --no-requeue
#SBATCH --get-user-env
#SBATCH --job-name="smart_test"
#SBATCH --partition=THIN
# or FHPC
#SBATCH --nodes=1
#SBATCH --exclusive
#SBATCH --time=00:10:00

module load architecture/AMD
module load openMPI/4.1.4/icc/2021.7.1

export code=/u/external/aiace9/tests/STREAM/code
export run_dir=$(pwd)

cd $code
make clean
make all

cd $run_dir

# Exports to improve placement:
# export OMP_NUM_THREADS=24
# export OMP_PLACES={0}:24:1
# export OMP_DISPLAY_ENV=true
# export OMP_DISPLAY_AFFINITY=true

# run the code:
$code/executable.exe

# try to impove placements with numactl
# numactl --hardware
# to discover the architecutre organization
# numactl -N # -m #
# -N to bind to a specific processor
# -m to bind to a specific memory region
# https://cvw.cac.cornell.edu/Hybrid/numactl