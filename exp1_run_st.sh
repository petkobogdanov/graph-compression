#!/bin/bash

#       Runs slice tree for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=2
num_partitions=2

for((g=1; g<=$num_graphs; g++))
do
    echo "graph_compression -g syn_$g.graph -v syn_$g.data -o output -c ST -p $num_partitions -s syn_$g.sizes -m $radius > out_st_$g.txt"
    ./graph_compression -g syn_$g.graph -v syn_$g.data -o output -c ST -p $num_partitions -s syn_$g.sizes -m $radius > out_st_$g.txt 
done
