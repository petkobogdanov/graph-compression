#!/bin/bash

#       Runs slice tree for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=2
num_partitions=2
num_vertices=(200000 400000 600000 800000 1000000)

for n in ${num_vertices[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    echo "graph_compression -g syn_$n\_$g.graph -v syn_$n\_$g.data -o output -c ST -p $num_partitions -s syn_$n\_$g.sizes -m $radius > out_st_$n\_$g.txt"
    ./graph_compression -g syn_$n\_$g.graph -v syn_$n\_$g.data -o output -c ST -p $num_partitions -s syn_$n\_$g.sizes -m $radius > out_st_$n\_$g.txt 
  done
done
