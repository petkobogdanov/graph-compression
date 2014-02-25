#!/bin/bash

#       Generates indexes for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=2

for((g=1; g<=$num_graphs; g++))
do
    ./graph_compression -g syn_$g.graph -v syn_$g.data -s syn_$g.sizes -t 8 -m 2
done
