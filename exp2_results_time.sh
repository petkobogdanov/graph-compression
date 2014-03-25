#!/bin/bash
#       Computes results for experiment 2.
#       @Arlei Silva

num_graphs=10
size=(10000 100000 1000000)
num_runs=10

#Computing approximation results for error reduction
algorithms=('stbs' 'stus')

for alg in ${algorithms[@]}
do
  rm avg_std_approx_error_reduction_size\_$alg.dat
  for sz in ${size[@]}
  do
    avg_approximation=0
    for((exp=1; exp<=$num_graphs; exp++))
    do
      optimal_reduction=`grep optimal_sse_reduction syn_$sz\_$exp.stats | cut -d ' ' -f3`
      optimal_reduction=`echo ${optimal_reduction} | sed -e 's/[eE]+*/\\*10\\^/'`
      for((run=1; run<=$num_runs; run++))
      do
        alg_reduction=`grep sse_reduction out_$alg\_\$sz\_$exp\_$run.txt | cut -d ' ' -f3`
        alg_reduction=`echo ${alg_reduction} | sed -e 's/[eE]+*/\\*10\\^/'`
        approximation=`echo "scale=10; $alg_reduction/$optimal_reduction" | bc`
        avg_approximation=`echo "scale=10; $avg_approximation+$approximation" | bc`
      done
    done
    avg_approximation=`echo "scale=10; $avg_approximation/($num_graphs*$num_runs)" | bc`
    std_approximation=0
    for((exp=1; exp<=$num_graphs; exp++))
    do
      optimal_reduction=`grep optimal_sse_reduction syn_$sz\_$exp.stats | cut -d ' ' -f3`
      for((run=1; run<=$num_runs; run++))
      do
        alg_reduction=`grep sse_reduction out\_$alg\_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
        alg_reduction=`echo ${alg_reduction} | sed -e 's/[eE]+*/\\*10\\^/'`
        approximation=`echo "scale=10; $alg_reduction/$optimal_reduction" | bc`
        std_approximation=`echo "scale=10; $std_approximation+($avg_approximation-$approximation)^2" | bc`
      done
    done
    std_approximation=`echo "scale=10; sqrt($std_approximation/($num_graphs*$num_runs))" | bc`
    echo "$sz	$avg_approximation	$std_approximation" >> avg_std_approx_error_reduction_size\_$alg.dat
  done
done

rm avg_std_approx_error_reduction_size_st.dat
for sz in ${size[@]}
do
  avg_approximation=0
  for((exp=1; exp<=$num_graphs; exp++))
  do
    optimal_reduction=`grep optimal_sse_reduction syn_$sz\_$exp.stats | cut -d ' ' -f3`
    optimal_reduction=`echo ${optimal_reduction} | sed -e 's/[eE]+*/\\*10\\^/'`
    alg_reduction=`grep sse_reduction out_st_$sz\_$exp.txt | cut -d ' ' -f3`
    alg_reduction=`echo ${alg_reduction} | sed -e 's/[eE]+*/\\*10\\^/'`
    approximation=`echo "scale=10; $alg_reduction/$optimal_reduction" | bc`
    avg_approximation=`echo "scale=10; $avg_approximation+$approximation" | bc`
  done
  avg_approximation=`echo "scale=10; $avg_approximation/$num_graphs" | bc`
  std_approximation=0
  
  for((exp=1; exp<=$num_graphs; exp++))
  do
    optimal_reduction=`grep optimal_sse_reduction syn_$sz\_$exp.stats | cut -d ' ' -f3`
    optimal_reduction=`echo ${optimal_reduction} | sed -e 's/[eE]+*/\\*10\\^/'`
    alg_reduction=`grep sse_reduction out_st_$sz\_$exp.txt | cut -d ' ' -f3`
    alg_reduction=`echo ${alg_reduction} | sed -e 's/[eE]+*/\\*10\\^/'`
    approximation=`echo "scale=10; $alg_reduction/$optimal_reduction" | bc`
    std_approximation=`echo "scale=10; $std_approximation+($avg_approximation-$approximation)^2" | bc`
  done
  std_approximation=`echo "scale=10; sqrt($std_approximation/$num_graphs)" | bc`
  echo "$sz	$avg_approximation	$std_approximation" >> avg_std_approx_error_reduction_size_st.dat
done

#Computing compression time
for alg in ${algorithms[@]}
do
  rm avg_std_compression_time_size\_$alg.dat
  for sz in ${size[@]}
  do
    avg_compression_time=0
    for((exp=1; exp<=$num_graphs; exp++))
    do
      for((run=1; run<=$num_runs; run++))
      do
        compression_time=`grep compression_time out\_$alg\_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
       avg_compression_time=`echo "scale=10; $avg_compression_time+$compression_time" | bc`
      done
    done
    avg_compression_time=`echo "scale=10; $avg_compression_time/($num_graphs*$num_runs)" | bc`
    std_compression_time=0
    for((exp=1; exp<=$num_graphs; exp++))
    do
      for((run=1; run<=$num_runs; run++))
      do
        compression_time=`grep compression_time out\_$alg\_$sz\_$exp\_$run.txt | cut -d ' ' -f3`
        std_compression_time=`echo "scale=10; $std_compression_time+($avg_compression_time-$compression_time)^2" | bc`
      done
    done
    std_compression_time=`echo "scale=10; sqrt($std_compression_time/($num_graphs*$num_runs))" | bc`
    echo "$sz	$avg_compression_time	$std_compression_time" >> avg_std_compression_time_size\_$alg.dat
  done
done

rm avg_std_compression_time_size_st.dat
for sz in ${size[@]}
do
  avg_compression_time=0
  for((exp=1; exp<=$num_graphs; exp++))
  do
    compression_time=`grep compression_time out_st_$sz\_$exp.txt | cut -d ' ' -f3`
    avg_compression_time=`echo "scale=10; $avg_compression_time+$compression_time" | bc`
  done
  avg_compression_time=`echo "scale=10; $avg_compression_time/$num_graphs" | bc`
  std_compression_time=0
  
  for((exp=1; exp<=$num_graphs; exp++))
  do
    compression_time=`grep compression_time out_st_$sz\_$exp.txt | cut -d ' ' -f3`
    std_compression_time=`echo "scale=10; $std_compression_time+($avg_compression_time-$compression_time)^2" | bc`
  done
  std_compression_time=`echo "scale=10; sqrt($std_compression_time/$num_graphs)" | bc`
  echo "$sz	$compression_time	$std_compression_time" >> avg_std_compression_time_size_st.dat
done

