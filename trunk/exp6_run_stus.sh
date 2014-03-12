#!/bin/bash

#       Runs slice tree with biased sampling for dblp
#       @Arlei Silva

radius=2
num_partitions=2
delta=0.1
num_runs=5
sampling_ratios=(0.05 0.10 0.15 0.20)
num_vertices=1291210
rho=(1.0)

for c in ${sampling_ratios[@]}
do
  for((r=1; r<=$num_runs; r++))
  do
    for h in ${rho[@]}
    do
      num_samples=`python -c "from math import ceil; print int(ceil($num_vertices*$c))"`
      echo "graph_compression -g dblp.graph -v dblp_dm.data -o output -c STUS -p $num_partitions -n $num_samples -d $delta -s dblp.sizes -m $radius -r $h > out_stus_$c\_$h\_$r.txt"
      ./graph_compression -g dblp.graph -v dblp_dm.data -o output -c STUS -p $num_partitions -n $num_samples -d $delta -s dblp.sizes -m $radius -r $h > out_stus_$c\_$h\_$r.txt
    done
  done
done
