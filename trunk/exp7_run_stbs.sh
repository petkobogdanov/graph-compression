#!/bin/bash

#       Runs slice tree with biased sampling for dblp
#       @Arlei Silva

radius=2
num_partitions=2
delta=0.1
num_runs=10
sampling_ratio=0.05
num_vertices=1291210
rho=(0.0 0.2 0.4 0.6 0.8 1.0)
num_samples=`python -c "from math import ceil; print int(ceil($num_vertices*$sampling_ratio))"`

for h in ${rho[@]}
do
  for((r=1; r<=$num_runs; r++))
  do
      echo "graph_compression -g dblp.graph -v dblp_dm.data -o output -c STBS -p $num_partitions -n $num_samples -d $delta -s dblp.sizes -m $radius -r $h > out_stbs_$h\_$r.txt"
      ./graph_compression -g dblp.graph -v dblp_dm.data -o output -c STBS -p $num_partitions -n $num_samples -d $delta -s dblp.sizes -m $radius -r $h > out_stbs_$h\_$r.txt
  done
done
