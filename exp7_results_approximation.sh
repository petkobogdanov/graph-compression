#!/bin/bash
#       Computes results for experiment 7.
#       @Arlei Silva

rho=(0.0 0.2 0.4 0.6 0.8 1.0)
num_runs=10

algorithms=('stbs')

opt_reduction=`grep sse_reduction out_st.txt | cut -d ' ' -f3`

#Computing compression time
for alg in ${algorithms[@]}
do
  rm avg_std_approximation_rho\_$alg.dat
  for r in ${rho[@]}
  do
    avg_approximation=0
    for((run=1; run<=$num_runs; run++))
    do
      reduction=`grep sse_reduction out\_$alg\_$r\_$run.txt | cut -d ' ' -f3`
      approximation=`echo "scale=10; $reduction/$opt_reduction" | bc`
      avg_approximation=`echo "scale=10; $avg_approximation+$approximation" | bc`
    done
    avg_approximation=`echo "scale=10; $avg_approximation/$num_runs" | bc`
    std_approximation=0
    for((run=1; run<=$num_runs; run++))
    do
      reduction=`grep sse_reduction out\_$alg\_$r\_$run.txt | cut -d ' ' -f3`
      approximation=`echo "scale=10; $reduction/$opt_reduction" | bc`
      std_approximation=`echo "scale=10; $std_approximation+($avg_approximation-$approximation)^2" | bc`
    done
    std_approximation=`echo "scale=10; sqrt($std_approximation/$num_runs)" | bc`
    echo "$r	$avg_approximation	$std_approximation" >> avg_std_approximation_rho\_$alg.dat
  done
done

