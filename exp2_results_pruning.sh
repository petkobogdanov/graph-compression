#!/bin/bash
#       Computes results for experiment 3.
#       @Arlei Silva

num_graphs=10
size=(10000 100000 1000000)
num_runs=10
delta=0.1

for sz in ${size[@]}
do
  avg_rate_pruned_1_stbs=0
  avg_rate_pruned_2_stbs=0
  avg_rate_pruned_stbs=0
  for((exp=1; exp<=$num_graphs; exp++))
  do
    for((run=1; run<=$num_runs; run++))
    do
      rate_pruned_stbs=`grep pruned_slices out_stbs_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      rate_pruned_stbs=`echo ${rate_pruned_stbs} | sed -e 's/[eE]+*/\\*10\\^/'`
      count_1=`grep count_bound_1 out_stbs_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      count_2=`grep count_bound_2 out_stbs_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      count_3=`grep count_bound_3 out_stbs_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      rate_1=`echo "scale=10; ($count_1+$count_2)/($count_1+$count_2+$count_3)" | bc`
      rate_2=`echo "scale=10; $count_3/($count_1+$count_2+$count_3)" | bc`
      rate_1=`echo "scale=10; $rate_1*$rate_pruned_stbs" | bc`
      rate_2=`echo "scale=10; $rate_2*$rate_pruned_stbs" | bc`
      avg_rate_pruned_stbs=`echo "scale=10; $avg_rate_pruned_stbs+$rate_pruned_stbs" | bc`
      avg_rate_pruned_1_stbs=`echo "scale=10; $avg_rate_pruned_1_stbs+$rate_1" | bc`
      avg_rate_pruned_2_stbs=`echo "scale=10; $avg_rate_pruned_2_stbs+$rate_2" | bc`
    done
  done
  avg_rate_pruned_1_stbs=`echo "scale=10; $avg_rate_pruned_1_stbs/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_2_stbs=`echo "scale=10; $avg_rate_pruned_2_stbs/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_stbs=`echo "scale=10; $avg_rate_pruned_stbs/($num_graphs*$num_runs)" | bc`
  echo "STIS $avg_rate_pruned_1_stbs $avg_rate_pruned_2_stbs" > avg_pruning_size_$sz\_stbs.dat
  
  avg_rate_pruned_1_stus=0
  avg_rate_pruned_2_stus=0
  avg_rate_pruned_stus=0
  for((exp=1; exp<=$num_graphs; exp++))
  do
    for((run=1; run<=$num_runs; run++))
    do
      rate_pruned_stus=`grep pruned_slices out_stus_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      rate_pruned_stus=`echo ${rate_pruned_stus} | sed -e 's/[eE]+*/\\*10\\^/'`
      count_1=`grep count_bound_1 out_stus_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      count_2=`grep count_bound_2 out_stus_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      count_3=`grep count_bound_3 out_stus_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
      rate_1=`echo "scale=10; ($count_1+$count_2)/($count_1+$count_2+$count_3)" | bc`
      rate_2=`echo "scale=10; $count_3/($count_1+$count_2+$count_3)" | bc`
      rate_1=`echo "scale=10; $rate_1*$rate_pruned_stus" | bc`
      rate_2=`echo "scale=10; $rate_2*$rate_pruned_stus" | bc`
      avg_rate_pruned_stus=`echo "scale=10; $avg_rate_pruned_stus+$rate_pruned_stus" | bc`
      avg_rate_pruned_1_stus=`echo "scale=10; $avg_rate_pruned_1_stus+$rate_1" | bc`
      avg_rate_pruned_2_stus=`echo "scale=10; $avg_rate_pruned_2_stus+$rate_2" | bc`
    done
  done
  avg_rate_pruned_1_stus=`echo "scale=10; $avg_rate_pruned_1_stus/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_2_stus=`echo "scale=10; $avg_rate_pruned_2_stus/($num_graphs*$num_runs)" | bc`
  avg_rate_pruned_stus=`echo "scale=10; $avg_rate_pruned_stus/($num_graphs*$num_runs)" | bc`

  echo "STUS $avg_rate_pruned_1_stus $avg_rate_pruned_2_stus" > avg_pruning_size_$sz\_stus.dat
done


