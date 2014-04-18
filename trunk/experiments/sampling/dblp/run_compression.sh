#!/bin/bash

#       Runs slice tree (several versions) for the synthetic graphs
#       @Arlei Silva

source default.sh

for f in ${dblp_data_files[@]}
do
#    echo "$graph_compression -g dblp.graph -v $f.data -o output -c ST -p $num_partitions_alg -s dblp.sizes -m $max_radius -x > out_st_$f.txt"
#    $graph_compression -g dblp.graph -v $f.data -o output -c ST -p $num_partitions_alg -s dblp.sizes -m $max_radius -x > out_st_$f.txt
    
    for((r=1; r<=$num_runs_sampling; r++))
    do
        postfix_samp=$f\_$r
        echo "$graph_compression -g dblp.graph -v $f.data -o output -c STBS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_fast_sampling -d $delta_fast_sampling -r $rho_fast_sampling -x > out_stbs_fast_$postfix_samp.txt"
        $graph_compression -g dblp.graph -v $f.data -o output -c STBS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_fast_sampling -d $delta_fast_sampling -r $rho_fast_sampling -x > out_stbs_fast_$postfix_samp.txt
        echo "$graph_compression -g dblp.graph -v $f.data -o output -c STBS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_slow_sampling -d $delta_slow_sampling -r $rho_slow_sampling -x > out_stbs_slow_$postfix_samp.txt"
        $graph_compression -g dblp.graph -v $f.data -o output -c STBS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_slow_sampling -d $delta_slow_sampling -r $rho_slow_sampling -x > out_stbs_slow_$postfix_samp.txt
        
	echo "$graph_compression -g dblp.graph -v $f.data -o output -c STUS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_fast_sampling -d $delta_fast_sampling -r $rho_fast_sampling -x > out_stus_fast_$postfix_samp.txt"
	$graph_compression -g dblp.graph -v $f.data -o output -c STUS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_fast_sampling -d $delta_fast_sampling -r $rho_fast_sampling -x > out_stus_fast_$postfix_samp.txt
        echo "$graph_compression -g dblp.graph -v $f.data -o output -c STUS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_slow_sampling -d $delta_slow_sampling -r $rho_slow_sampling -x > out_stus_slow_$postfix_samp.txt"
        $graph_compression -g dblp.graph -v $f.data -o output -c STUS -p $num_partitions_alg -s dblp.sizes -m $max_radius -n $rate_slow_sampling -d $delta_slow_sampling -r $rho_slow_sampling -x > out_stus_slow_$postfix_samp.txt
    done
done
