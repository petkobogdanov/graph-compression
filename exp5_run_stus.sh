#!/bin/bash

#       Runs slice tree with uniform sampling for the synthetic graphs
#       @Arlei Silva

radius=2
num_partitions=2
num_samples=10000
delta=0.1
num_runs=10
reduction_ratios=(0.0 0.25 0.5 0.75 1.0)
num_graphs=10

for c in ${reduction_ratios[@]}
do
  for((r=1; r<=$num_runs; r++))
  do
    echo "graph_compression -g syn_$c\_$g.graph -v syn_$c\_$g.data -o output -c STUS -p $num_partitions -n $num_samples -d $delta -s syn_$c\_$g.sizes -m $radius > out_stus_$c\_$g\_$r.txt"
      ./graph_compression -g syn_$c\_$g.graph -v syn_$c\_$g.data -o output -c STUS -p $num_partitions -n $num_samples -d $delta -s syn_$c\_$g.sizes -m $radius > out_stus_$c\_$g\_$r.txt
  done
done
