#!/bin/bash

#       Runs wavelets for the human dataset
#       @Arlei Silva

num_snapshots=114
num_partitions=(2 4 8 16 32)

for((snp=0; snp<$num_snapshots; snp++))
do
    for np in ${num_partitions[@]}
    do
      ./graph_compression -g human_$snp.graph -v human_$snp.data -o human_$snp.bin -c WV -p $np >> out_wv_$snp\_$np.dat
    done
done
