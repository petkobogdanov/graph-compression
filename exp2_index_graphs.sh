#!/bin/bash

#       Generates indexes for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=2
num_vertices=(200000 400000 600000 800000 1000000)

for n in ${num_vertices[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    echo "graph_compression -g syn_$g.graph -v syn_$g.data -s syn_$g.sizes -t 8 -m $radius"
    ./graph_compression -g syn_$n\_$g.graph -v syn_$n\_$g.data -s syn_$n\_$g.sizes -t 8 -m $radius
  done
done
