#!/bin/bash

#       Runs slice tree (several versions) for the synthetic graphs
#       @Arlei Silva

source default.sh

rm $results_pruning.dat

echo "data      STBS_SLOW       STBS_FAST" >> $results_pruning.dat

for f in ${dblp_data_files[@]}
do
  avg_stbs_fast=0
  avg_stbs_slow=0
  postfix=$f
  for((r=1; r<=$num_runs_sampling; r++))
  do
        postfix_samp=$postfix\_$r
        pruning_1=`grep count_bound_1 out_stbs_fast_$postfix_samp.txt | cut -d ' ' -f3`
        pruning_2=`grep count_bound_2 out_stbs_fast_$postfix_samp.txt | cut -d ' ' -f3`
        pruning_3=`grep count_bound_3 out_stbs_fast_$postfix_samp.txt | cut -d ' ' -f3`
	rate=`echo "scale=10; $pruning_3/($pruning_1+$pruning_2+$pruning_3)" | bc`
        
	avg_stbs_fast=`echo "scale=10; $avg_stbs_fast+$rate" | bc`
        
        pruning_1=`grep count_bound_1 out_stbs_slow_$postfix_samp.txt | cut -d ' ' -f3`
        pruning_2=`grep count_bound_2 out_stbs_slow_$postfix_samp.txt | cut -d ' ' -f3`
        pruning_3=`grep count_bound_3 out_stbs_slow_$postfix_samp.txt | cut -d ' ' -f3`
	rate=`echo "scale=10; $pruning_3/($pruning_1+$pruning_2+$pruning_3)" | bc`
        avg_stbs_slow=`echo "scale=10; $avg_stbs_slow+$rate" | bc`
  done
  avg_stbs_fast=`echo "scale=10; $avg_stbs_fast/$num_runs_sampling" | bc`
  avg_stbs_slow=`echo "scale=10; $avg_stbs_slow/$num_runs_sampling" | bc`

  echo "$f	$avg_stbs_slow	$avg_stbs_fast" >> $results_pruning.dat
done
