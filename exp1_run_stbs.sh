#!/bin/bash

#       Runs slice tree with biased sampling for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=2
num_partitions=2
num_samples=(5000 10000 15000 20000 25000)
delta=(0.05 0.1)
num_runs=10

for s in ${num_samples[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    for((r=1; r<=$num_runs; r++))
    do
      for d in ${delta[@]}
      do
        ./graph_compression -g syn_$g.graph -v syn_$g.data -o output -c STBS -p $num_partitions -n $s -d $d -s syn_$g.sizes -m $radius > out_stbs_$g\_$n\_$d\_$r.txt
      done
    done
  done
done
