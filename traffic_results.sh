#!/bin/bash

#       Computes the average, standard deviation, and minimum 
#	values of several statistics of the compression algorithms
#       @Arlei Silva

num_snapshots=8640
algorithms=('slice_tree' 'average_linkage' 'wavelets')
budgets=(154 308 616 1231 2462 4923)
statistics=('sse', 'compression_time')

#Computing other statistics
for alg in ${algorithms[@]}
do
  for stat in ${statistics[@]}
  do
    rm avg_std_$stat\_$alg.dat
    for bgt in ${budgets[@]}
    do
      n=0
      avg=0
      for((snp=0; snp<$num_snapshots; snp++))
      do
        rem=`echo "scale=0; $snp%12" | bc`
        if [ "$rem" -eq "0" ]
	then
          value_stat=`grep "^$stat " out\_$alg\_$snp\_$bgt.dat | cut -d ' ' -f3`
	  value_stat=`echo ${value_stat} | sed -e 's/[eE]+*/\\*10\\^/'`
	  avg=`echo "scale=10; $avg+$value_stat" | bc`
	  n=$n+1
	fi
      done
      avg=`echo "scale=10; $avg/$n" | bc`
      std=0
      for((snp=0; snp<$num_snapshots; snp++))
      do
        if [ "$rem" -eq "0" ]
	then
          value_stat=`grep "^$stat " out\_$alg\_$snp\_$bgt.dat | cut -d ' ' -f3`
	  value_stat=`echo ${value_stat} | sed -e 's/[eE]+*/\\*10\\^/'`
          std=`echo "scale=10; $std+($avg-$value_stat)^2" | bc`
	fi
      done
      std=`echo "scale=10; sqrt($std/$n)" | bc`
      echo "$bgt	$avg	$std" >> avg_std_$stat\_$alg.dat
    done
  done
done

