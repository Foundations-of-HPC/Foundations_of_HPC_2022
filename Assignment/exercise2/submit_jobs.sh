#!/bin/bash
# Clear output file before looping input data
rm *output.txt

# Loop through each line in the input file
while IFS= read -r arguments; do
    # Submit a SLURM job with the current set of arguments
    sbatch script.sh $arguments
done < arguments.txt

