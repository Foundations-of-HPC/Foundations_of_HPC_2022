#!/bin/bash
#PBS -N IOZONE_test 
#PBS -l walltime=01:00:00
#PBS -l nodes=1:ppn=64
#PBS -q mhpc


SCRATCH=/lustre/brandino
IOZONE=/u/mhpc/brandino/iozone3_429/src/current/iozone

for i in 0 1; do
        mkdir $SCRATCH/ost_$i
        cd $SCRATCH/ost_$i
        lfs setstripe -c 1 -i $i .
        for SIZE in 8g 64g 128g 150g; do
                $IOZONE -i 1 -i 0 -s $SIZE -r 1M -+q 20 -f ./$SIZE_OST_$i >> output_ost_$i
        done
        cd ..
        rm -rf ost_$i
done
