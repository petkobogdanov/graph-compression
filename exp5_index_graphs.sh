#!/bin/bash

#       Generates indexes for the synthetic graphs
#       @Arlei Silva

radius=2
reduction_ratios=(0.0 0.25 0.5 0.75 1.0)

for c in ${reduction_ratios[@]}
do
   echo "graph_compression -g syn_$c.graph -v syn_$c.data -s syn_$c.sizes -t 4 -m $radius"
   ./graph_compression -g syn_$c.graph -v syn_$c.data -s syn_$c.sizes -t 4 -m $radius
done
