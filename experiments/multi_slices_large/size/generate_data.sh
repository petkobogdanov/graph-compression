#!/bin/bash

#       Generates a number of synthetic graphs with given properties using a 
#	synthetic graph generator
#       @Arlei Silva

source default.sh

for p in ${param_sizes[@]}
do
    for((g=1; g<=$num_graphs; g++))
    do
    	prefix=$graph_name_prefix\_$g\_$p
        echo "python $graph_generator -o $prefix -v $p -e $num_edges -p $num_partitions_data -r $max_radius -c $sse_reduction -s $sse"
        python $graph_generator -o $prefix -v $p -e $num_edges -p $num_partitions_data -r $max_radius -c $sse_reduction -s $sse
    done
done
