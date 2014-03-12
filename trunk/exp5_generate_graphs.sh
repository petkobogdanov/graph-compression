#!/bin/bash

#       Generates a number of synthetic graphs with given properties using a synthetic graph generator
#       @Arlei Silva

radius=2
max_value=1
num_edges=5
num_partitions=2
num_vertices=100000
num_graphs=10
reduction_ratios=(0.0 0.25 0.5 0.75 1.0)

for((g=1; g<=$num_graphs; g++))
do
  for c in ${reduction_ratios[@]}
  do
    sse=$num_vertices
    sse_reduction=`echo "scale=10; $num_vertices*$c" | bc`
    echo "python graph_generator.py -o syn_$c\_$g -v $num_vertices -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse"
    python graph_generator.py -o syn_$c\_$g -v $num_vertices -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse
  done
done
