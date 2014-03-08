#!/bin/bash

#       Runs slice tree for the synthetic graphs
#       @Arlei Silva

radius=2
num_partitions=2
num_vertices=(200000 400000 800000 1600000)

for n in ${num_vertices[@]}
do
  echo "graph_compression -g syn_$n.graph -v syn_$n.data -o output -c ST -p $num_partitions -s syn_$n.sizes -m $radius > out_st_$n.txt"
  ./graph_compression -g syn_$n.graph -v syn_$n.data -o output -c ST -p $num_partitions -s syn_$n.sizes -m $radius > out_st_$n.txt 
done
