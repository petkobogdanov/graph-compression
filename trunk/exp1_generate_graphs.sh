#!/bin/bash

#       Generates a number of synthetic graphs with given properties using a synthetic graph generator
#       @Arlei Silva

num_graphs=10
radius=2
max_value=1
num_edges=5
num_partitions=2
num_vertices=400000
sse=400000
sse_reduction=200000

for((g=1; g<=$num_graphs; g++))
do
    echo "python graph_generator.py -o syn_$g -v $num_vertices -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse"
    python graph_generator.py -o syn_$g -v $num_vertices -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse_reduction
done
