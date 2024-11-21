#!/bin/bash
#SBATCH --job-name demo_serial
#SBATCH --tasks-per-node 1
#SBATCH --nodelist node4

cd $SLURM_SUBMIT_DIR
make
./main
