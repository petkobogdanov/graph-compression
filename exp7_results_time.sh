#!/bin/bash
#       Computes results for experiment 7.
#       @Arlei Silva

rho=(0.0 0.2 0.4 0.6 0.8 1.0)
num_runs=10

algorithms=('stbs')

#Computing compression time
for alg in ${algorithms[@]}
do
  rm avg_std_compression_time_rho\_$alg.dat
  for r in ${rho[@]}
  do
    avg_compression_time=0
    for((run=1; run<=$num_runs; run++))
    do
      compression_time=`grep compression_time out\_$alg\_$r\_$run.txt | cut -d ' ' -f3`
      avg_compression_time=`echo "scale=10; $avg_compression_time+$compression_time" | bc`
    done
    avg_compression_time=`echo "scale=10; $avg_compression_time/$num_runs" | bc`
    std_compression_time=0
    for((run=1; run<=$num_runs; run++))
    do
      compression_time=`grep compression_time out\_$alg\_$r\_$run.txt | cut -d ' ' -f3`
      std_compression_time=`echo "scale=10; $std_compression_time+($avg_compression_time-$compression_time)^2" | bc`
    done
    std_compression_time=`echo "scale=10; sqrt($std_compression_time/$num_runs)" | bc`
    echo "$r	$avg_compression_time	$std_compression_time" >> avg_std_compression_time_rho\_$alg.dat
  done
done

rm avg_std_compression_time_rho_st.dat
for r in ${rho[@]}
do
  avg_compression_time=`grep compression_time out_st.txt | cut -d ' ' -f3`
  std_compression_time=0
  echo "$r	$avg_compression_time	$std_compression_time" >> avg_std_compression_time_rho_st.dat
done

