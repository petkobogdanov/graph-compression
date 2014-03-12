#!/bin/bash

#       Runs slice tree for the synthetic graphs
#       @Arlei Silva

radius=2
num_partitions=2
num_vertices=(100000 200000 300000 400000 500000)

for n in ${num_vertices[@]}
do
  echo "graph_compression -g syn_$n.graph -v syn_$n.data -o output -c ST -p $num_partitions -s syn_$n.sizes -m $radius > out_st_$n.txt"
  ./graph_compression -g syn_$n.graph -v syn_$n.data -o output -c ST -p $num_partitions -s syn_$n.sizes -m $radius > out_st_$n.txt 
done
