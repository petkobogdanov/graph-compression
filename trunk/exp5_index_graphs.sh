#!/bin/bash

#       Generates indexes for the synthetic graphs
#       @Arlei Silva

radius=2
reduction_ratios=(0.0 0.25 0.5 0.75 1.0)
num_graphs=10

for((g=1; g<=$num_graphs; g++))
do
  for c in ${reduction_ratios[@]}
  do
    echo "graph_compression -g syn_$c\_$g.graph -v syn_$c\_$g.data -s syn_$c\_$g.sizes -t 4 -m $radius"
    ./graph_compression -g syn_$c\_$g.graph -v syn_$c\_$g.data -s syn_$c\_$g.sizes -t 4 -m $radius
  done
done
