#!/bin/bash

#       Runs slice tree with biased sampling for dblp
#       @Arlei Silva

radius=2
num_partitions=2
delta=0.1
num_runs=10
sampling_ratios=(0.02 0.04 0.06 0.08 0.1 0.12 0.14)
num_vertices=1291210
rho=(0.75 1.0)

for c in ${sampling_ratios[@]}
do
  for((r=1; r<=$num_runs; r++))
  do
    for h in ${rho[@]}
    do
      num_samples=`python -c "from math import ceil; print int(ceil($num_vertices*$c))"`
      echo "graph_compression -g dblp.graph -v dblp.data -o output -c STBS -p $num_partitions -n $num_samples -d $delta -s dblp.sizes -m $radius -r $h > out_stus_$c\_$h\_$r.txt"
      ./graph_compression -g dblp.graph -v dblp.data -o output -c STBS -p $num_partitions -n $num_samples -d $delta -s dblp.sizes -m $radius -r $h > out_stus_$c\_$h\_$r.txt
    done
  done
done
