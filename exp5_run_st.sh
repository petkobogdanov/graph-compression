#!/bin/bash

#       Runs slice tree for the synthetic graphs
#       @Arlei Silva

radius=2
num_partitions=2
reduction_ratios=(0.0 0.25 0.5 0.75 1.0)
num_graphs=10

for((g=1; g<=$num_graphs; g++))
do
  for c in ${reduction_ratios[@]}
  do
    echo "graph_compression -g syn_$c\_$g.graph -v syn_$c\_$g.data -o output -c ST -p $num_partitions -s syn_$n.sizes -m $radius > out_st_$c\_$g.txt"
    ./graph_compression -g syn_$c\_$g.graph -v syn_$c\_$g.data -o output -c ST -p $num_partitions -s syn_$c\_$g.sizes -m $radius > out_st_$c\_$g.txt 
  done
done
