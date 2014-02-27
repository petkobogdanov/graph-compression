#!/bin/bash

#       Runs slice tree with uniform sampling for the synthetic graphs
#       @Arlei Silva

num_graphs=10
num_partitions=2
num_samples=10000
delta=(0.05 0.1)
num_runs=10
radius=(1 2 3)

for u in ${radius[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    for((r=1; r<=$num_runs; r++))
    do
      for d in ${delta[@]}
      do
        echo "graph_compression -g syn_$u\_$g.graph -v syn_$u\_$g.data -o output -c STUS -p $num_partitions -n $num_samples -d $d -s syn_$u\_$g.sizes -m 3 > out_stus_$u\_$g\_$d\_$r.txt"
        ./graph_compression -g syn_$u\_$g.graph -v syn_$u\_$g.data -o output -c STUS -p $num_partitions -n $num_samples -d $d -s syn_$u\_$g.sizes -m 3 > out_stus_$u\_$g\_$d\_$r.txt
      done
    done
  done
done
