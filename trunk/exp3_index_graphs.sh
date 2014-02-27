#!/bin/bash

#       Generates indexes for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=(1 2 3)

for u in ${radius[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    echo "graph_compression -g syn_$u\_$g.graph -v syn_$u\_$g.data -s syn_$u\_$g.sizes -t 8 -m 3"
    ./graph_compression -g syn_$u\_$g.graph -v syn_$u\_$g.data -s syn_$u\_$g.sizes -t 8 -m 3
  done
done
