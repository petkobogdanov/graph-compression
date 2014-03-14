#!/bin/bash

#       Runs slice tree with uniform sampling for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=2
num_partitions=2
sampling_rate=(0.01 0.02 0.04 0.08 0.16)
num_runs=10
delta=0.1

for s in ${sampling_rate[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    for((r=1; r<=$num_runs; r++))
    do
        num_samples=`python -c "from math import ceil; print int(ceil(400000*$s))"`
        echo "graph_compression -g syn_$g.graph -v syn_$g.data -o output -c STUS -p $num_partitions -n $num_samples -d $delta -s syn_$g.sizes -m $radius > out_stus_$g\_$s\_$r.txt"
        ./graph_compression -g syn_$g.graph -v syn_$g.data -o output -c STUS -p $num_partitions -n $num_samples -d $delta -s syn_$g.sizes -m $radius > out_stus_$g\_$s\_$r.txt
    done
  done
done
