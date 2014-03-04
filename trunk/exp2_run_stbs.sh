#!/bin/bash

#       Runs slice tree with biased sampling for the synthetic graphs
#       @Arlei Silva

num_graphs=10
radius=2
num_partitions=2
sampling_ratio=0.1
delta=(0.05 0.1)
num_runs=10
num_vertices=(200000 400000 600000 800000 1000000)

for n in ${num_vertices[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    for((r=1; r<=$num_runs; r++))
    do
      for d in ${delta[@]}
      do
        num_samples=`echo "scale=10; $n*$sampling_ratio" | bc`
        echo "graph_compression -g syn_$n\_$g.graph -v syn_$n\_$g.data -o output -c STBS -p $num_partitions -n $num_samples -d $d -s syn_$g.sizes -m $radius > out_stbs_$n\_$g\_$d\_$r.txt"
        ./graph_compression -g syn_$n\_$g.graph -v syn_$n\_$g.data -o output -c STBS -p $num_partitions -n $num_samples -d $d -s syn_$n\_$g.sizes -m $radius > out_stbs_$n\_$g\_$d\_$r.txt
      done
    done
  done
done
