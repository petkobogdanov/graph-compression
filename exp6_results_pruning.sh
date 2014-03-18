#!/bin/bash
#       Computes results for experiment 6.
#       @Arlei Silva

sampling_rate=(0.05 0.10 0.15 0.20)
num_runs=10
delta=1.0

for s in ${sampling_rate[@]}
do
  avg_rate_pruned_1_stbs=0
  avg_rate_pruned_2_stbs=0
  avg_rate_pruned_stbs=0
  for((run=1; run<=$num_runs; run++))
  do
    rate_pruned_stbs=`grep pruned_slices out_stbs_$s\_$delta\_$run.txt | cut -d ' ' -f3`
    rate_pruned_stbs=`echo ${rate_pruned_stbs} | sed -e 's/[eE]+*/\\*10\\^/'`
    count_1=`grep count_bound_1 out_stbs_$s\_$delta\_$run.txt | cut -d ' ' -f3`
    count_2=`grep count_bound_2 out_stbs_$s\_$delta\_$run.txt | cut -d ' ' -f3`
    count_3=`grep count_bound_3 out_stbs_$s\_$delta\_$run.txt | cut -d ' ' -f3`
    rate_1=`echo "scale=10; ($count_1+$count_2)/($count_1+$count_2+$count_3)" | bc`
    rate_2=`echo "scale=10; $count_3/($count_1+$count_2+$count_3)" | bc`
    rate_1=`echo "scale=10; $rate_1*$rate_pruned_stbs" | bc`
    rate_2=`echo "scale=10; $rate_2*$rate_pruned_stbs" | bc`
    avg_rate_pruned_stbs=`echo "scale=10; $avg_rate_pruned_stbs+$rate_pruned_stbs" | bc`
    avg_rate_pruned_1_stbs=`echo "scale=10; $avg_rate_pruned_1_stbs+$rate_1" | bc`
    avg_rate_pruned_2_stbs=`echo "scale=10; $avg_rate_pruned_2_stbs+$rate_2" | bc`
  done
  avg_rate_pruned_1_stbs=`echo "scale=10; $avg_rate_pruned_1_stbs/$num_runs" | bc`
  avg_rate_pruned_2_stbs=`echo "scale=10; $avg_rate_pruned_2_stbs/$num_runs" | bc`
  avg_rate_pruned_stbs=`echo "scale=10; $avg_rate_pruned_stbs/$num_runs" | bc`
  echo "STIS $avg_rate_pruned_1_stbs $avg_rate_pruned_2_stbs" > avg_pruning_sampling_$s\_stbs.dat
done


