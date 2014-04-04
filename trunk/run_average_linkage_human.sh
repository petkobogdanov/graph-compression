#!/bin/bash

#       Runs average linkage for the human dataset
#       @Arlei Silva

num_snapshots=114
num_partitions=(2 4 8 16 32)

for((snp=0; snp<$num_snapshots; snp++))
do
    for np in ${num_partitions[@]}
    do
      ./graph_compression -g human_$snp.graph -v human_$snp.data -o human_$snp.bin -c AL -p $np >> out_al_$snp\_$np.dat
    done
done
