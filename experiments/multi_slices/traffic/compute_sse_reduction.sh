#!/bin/bash

#       Runs slice tree (several versions) for the synthetic graphs
#       @Arlei Silva

source default.sh

rm $results_sse_reduction.dat

for f in ${data_files[@]}
do
  postfix=$f  
  st=`grep sse_reduction out_st_$postfix.txt | cut -d ' ' -f3`
  st=`echo ${st} | sed -e 's/[eE]+*/\\*10\\^/'`
  
  wvb=`grep sse_reduction out_wvb_$postfix.txt | cut -d ' ' -f3`
  wvb=`echo ${wvb} | sed -e 's/[eE]+*/\\*10\\^/'`
  
  al=`grep sse_reduction out_al_$postfix.txt | cut -d ' ' -f3`
  al=`echo ${al} | sed -e 's/[eE]+*/\\*10\\^/'`

  echo "$f	$wvb	$al	$st" >> $results_sse_reduction.dat
done
  
