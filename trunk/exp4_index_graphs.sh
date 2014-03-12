#!/bin/bash

#       Generates indexes for the synthetic graphs
#       @Arlei Silva

radius=2
num_vertices=(100000 200000 300000 400000 500000)

for n in ${num_vertices[@]}
do
   echo "graph_compression -g syn_$n.graph -v syn_$n.data -s syn_$n.sizes -t 4 -m $radius"
   ./graph_compression -g syn_$n.graph -v syn_$n.data -s syn_$n.sizes -t 4 -m $radius
done
