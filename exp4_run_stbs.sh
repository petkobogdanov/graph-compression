#!/bin/bash

#       Runs slice tree with biased sampling for the synthetic graphs
#       @Arlei Silva

radius=2
num_partitions=2
sampling_rates=(0.02 0.04 0.06 0.08 0.1 0.12 0.14 0.16)
delta=0.1
num_runs=10
num_vertices=(200000 400000 800000 1600000)

for s in ${sampling_rates[@]}
do
  for n in ${num_vertices[@]}
  do
    for((r=1; r<=$num_runs; r++))
    do
      num_samples=`python -c "from math import ceil; print int(ceil($n*$s))"`
      echo "graph_compression -g syn_$n.graph -v syn_$n.data -o output -c STBS -p $num_partitions -n $num_samples -d $delta -s syn_$n.sizes -m $radius > out_stbs_$n\_$s\_$r.txt"
#      ./graph_compression -g syn_$n.graph -v syn_$n.data -o output -c STBS -p $num_partitions -n $num_samples -d $delta -s syn_$n.sizes -m $radius > out_stbs_$n\_$s\_$r.txt
    done
  done
done
