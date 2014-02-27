#!/bin/bash

#       Runs slice tree for the synthetic graphs
#       @Arlei Silva

num_graphs=10
num_partitions=2
radius=(1 2 3)

for u in ${radius[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    echo "graph_compression -g syn_$u\_$g.graph -v syn_$u\_$g.data -o output -c ST -p $num_partitions -s syn_$u\_$g.sizes -m 3 > out_st_$u\_$g.txt"
    ./graph_compression -g syn_$u\_$g.graph -v syn_$u\_$g.data -o output -c ST -p $num_partitions -s syn_$u\_$g.sizes -m 3 > out_st_$u\_$g.txt 
  done
done
