#!/bin/bash

#       Generates a number of synthetic graphs with given properties using a synthetic graph generator
#       @Arlei Silva

num_graphs=1
radius=2
max_value=1
num_edges=5
num_partitions=2
num_vertices=(10000 100000 1000000 10000000)

for n in ${num_vertices[@]}
do
  for((g=1; g<=$num_graphs; g++))
  do
    sse=$n
    sse_reduction=`echo "scale=10; $n/2" | bc`
    echo "python graph_generator.py -o syn_$n\_$g -v $n -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse"
    python graph_generator.py -o syn_$n\_$g -v $n -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse
  done
done
