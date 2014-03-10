#!/bin/bash

#       Runs slice tree for the synthetic graphs
#       @Arlei Silva

radius=2
num_partitions=2
reduction_ratios=(0.0 0.25 0.5 0.75 1.0)

for c in ${reduction_ratios[@]}
do
  echo "graph_compression -g syn_$c.graph -v syn_$c.data -o output -c ST -p $num_partitions -s syn_$n.sizes -m $radius > out_st_$c.txt"
  ./graph_compression -g syn_$c.graph -v syn_$c.data -o output -c ST -p $num_partitions -s syn_$c.sizes -m $radius > out_st_$c.txt 
done
