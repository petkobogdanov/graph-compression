#!/bin/bash
#       Computes results for experiment 6.
#       @Arlei Silva

sampling_rate=(0.05 0.10 0.15 0.20)
num_runs=10
delta=1.0

algorithms=('stbs' 'stus')

#Computing compression time
for alg in ${algorithms[@]}
do
  rm avg_std_compression_time_samples\_$alg.dat
  for s in ${sampling_rate[@]}
  do
    avg_compression_time=0
    for((run=1; run<=$num_runs; run++))
    do
      compression_time=`grep compression_time out\_$alg\_$s\_$delta\_$run.txt | cut -d ' ' -f3`
      avg_compression_time=`echo "scale=10; $avg_compression_time+$compression_time" | bc`
    done
    avg_compression_time=`echo "scale=10; $avg_compression_time/$num_runs" | bc`
    std_compression_time=0
    for((run=1; run<=$num_runs; run++))
    do
      compression_time=`grep compression_time out\_$alg\_$s\_$delta\_$run.txt | cut -d ' ' -f3`
      std_compression_time=`echo "scale=10; $std_compression_time+($avg_compression_time-$compression_time)^2" | bc`
    done
    std_compression_time=`echo "scale=10; sqrt($std_compression_time/$num_runs)" | bc`
    echo "$s	$avg_compression_time	$std_compression_time" >> avg_std_compression_time_samples\_$alg.dat
  done
done

rm avg_std_compression_time_samples_st.dat
for s in ${sampling_rate[@]}
do
  avg_compression_time=`grep compression_time out_st.txt | cut -d ' ' -f3`
  std_compression_time=0
  echo "$s	$avg_compression_time	$std_compression_time" >> avg_std_compression_time_samples_st.dat
done

