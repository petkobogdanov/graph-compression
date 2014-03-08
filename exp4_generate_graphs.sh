#!/bin/bash

#       Generates a number of synthetic graphs with given properties using a synthetic graph generator
#       @Arlei Silva

radius=2
max_value=1
num_edges=5
num_partitions=2
num_vertices=(200000 400000 800000 1600000)
#sampling_ratios=(0.02 0.04 0.06 0.08 0.1 0.12 0.14 0.16)

for n in ${num_vertices[@]}
do
  sse=$n
  sse_reduction=`echo "scale=10; $n/2" | bc`
  echo "python graph_generator.py -o syn_$n -v $n -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse"
  python graph_generator.py -o syn_$n -v $n -e $num_edges -p $num_partitions -r $radius -m $max_value -c $sse_reduction -s $sse
done
