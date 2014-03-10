#!/bin/bash

#       Runs slice tree for dblp
#       @Arlei Silva

radius=2
num_partitions=2

echo "graph_compression -g dblp.graph -v dblp_dm.data -o output -c ST -p $num_partitions -s dblp.sizes -m $radius > out_st.txt"
./graph_compression -g dblp.graph -v dblp_dm.data -o output -c ST -p $num_partitions -s dblp.sizes -m $radius > out_st.txt
