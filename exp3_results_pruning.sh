#!/bin/bash
#       Computes results for experiment 3.
#       @Arlei Silva

num_graphs=10
radius=(1 2 3)
num_runs=10
delta=0.1

rm avg_pruning.dat
echo "X STUS_1 STUS_2  STUS_3 STBS_1 STBS_2 STBS_3" >> avg_pruning.dat
for u in ${radius[@]}
do
  avg_rate_pruned_1_stbs=0
  avg_rate_pruned_2_stbs=0
  avg_rate_pruned_3_stbs=0
  for((exp=1; exp<=$num_graphs; exp++))
  do
    for((run=1; run<=$num_runs; run++))
    do
      rate_pruned=`grep pruned_slices out_stbs_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      rate_pruned=`echo ${rate_pruned} | sed -e 's/[eE]+*/\\*10\\^/'`
      count_1=`grep count_bound_1 out_stbs_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      count_2=`grep count_bound_2 out_stbs_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      count_3=`grep count_bound_3 out_stbs_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      rate_1=`echo "scale=10; $count_1/($count_1+$count_2+$count_3)" | bc`
      rate_2=`echo "scale=10; $count_2/($count_1+$count_2+$count_3)" | bc`
      rate_3=`echo "scale=10; $count_3/($count_1+$count_2+$count_3)" | bc`
      rate_1=`echo "scale=10; $rate_1*$rate_pruned" | bc`
      rate_2=`echo "scale=10; $rate_2*$rate_pruned" | bc`
      rate_3=`echo "scale=10; $rate_3*$rate_pruned" | bc`
      avg_rate_pruned_1_stbs=`echo "scale=10; $avg_rate_pruned_1_stbs+$rate_1" | bc`
      avg_rate_pruned_2_stbs=`echo "scale=10; $avg_rate_pruned_2_stbs+$rate_2" | bc`
      avg_rate_pruned_3_stbs=`echo "scale=10; $avg_rate_pruned_3_stbs+$rate_3" | bc`
    done
  done
  avg_rate_pruned_1_stbs=`echo "scale=10; $avg_rate_pruned_1_stbs/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_2_stbs=`echo "scale=10; $avg_rate_pruned_2_stbs/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_3_stbs=`echo "scale=10; $avg_rate_pruned_3_stbs/($num_graphs*$num_runs)" | bc`
  
  avg_rate_pruned_1_stus=0
  avg_rate_pruned_2_stus=0
  avg_rate_pruned_3_stus=0
  for((exp=1; exp<=$num_graphs; exp++))
  do
    for((run=1; run<=$num_runs; run++))
    do
      rate_pruned=`grep pruned_slices out_stus_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      rate_pruned=`echo ${rate_pruned} | sed -e 's/[eE]+*/\\*10\\^/'`
      count_1=`grep count_bound_1 out_stus_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      count_2=`grep count_bound_2 out_stus_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      count_3=`grep count_bound_3 out_stus_$u\_$exp\_$delta\_$run.txt | cut -d ' ' -f3`
      rate_1=`echo "scale=10; $count_1/($count_1+$count_2+$count_3)" | bc`
      rate_2=`echo "scale=10; $count_2/($count_1+$count_2+$count_3)" | bc`
      rate_3=`echo "scale=10; $count_3/($count_1+$count_2+$count_3)" | bc`
      rate_1=`echo "scale=10; $rate_1*$rate_pruned" | bc`
      rate_2=`echo "scale=10; $rate_2*$rate_pruned" | bc`
      rate_3=`echo "scale=10; $rate_3*$rate_pruned" | bc`
      avg_rate_pruned_1_stus=`echo "scale=10; $avg_rate_pruned_1_stus+$rate_1" | bc`
      avg_rate_pruned_2_stus=`echo "scale=10; $avg_rate_pruned_2_stus+$rate_2" | bc`
      avg_rate_pruned_3_stus=`echo "scale=10; $avg_rate_pruned_3_stus+$rate_3" | bc`
    done
  done
  avg_rate_pruned_1_stus=`echo "scale=10; $avg_rate_pruned_1_stus/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_2_stus=`echo "scale=10; $avg_rate_pruned_2_stus/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_3_stus=`echo "scale=10; $avg_rate_pruned_3_stus/($num_graphs*$num_runs)" | bc`

  echo "$u $avg_rate_pruned_1_stus $avg_rate_pruned_2_stus $avg_rate_pruned_3_stus $avg_rate_pruned_1_stbs $avg_rate_pruned_2_stbs $avg_rate_pruned_3_stbs" >> avg_pruning.dat
done

