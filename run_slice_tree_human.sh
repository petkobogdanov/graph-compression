#!/bin/bash

#       Runs slice tree for the human dataset
#       @Arlei Silva

num_snapshots=114
num_partitions=(2 4 8 16 32)

for((snp=0; snp<$num_snapshots; snp++))
do 
    ./graph_compression -g human_$snp.graph -v human_$snp.data -s human_$snp.sizes
done

for((snp=0; snp<$num_snapshots; snp++))
do
    for np in ${num_partitions[@]}
    do
      ./graph_compression -g human_$snp.graph -v human_$snp.data -o human_$snp.bin -c ST -p $np -s human_$snp.sizes -i >> out_st_$snp\_$np.dat
    done
done
