set term postscript eps 40 enhanced color solid
set style fill solid 1.00 border
set encoding iso_8859_1
set output "pruning_size_syn.eps"
set xlabel "||V||"
set ylabel "% pruning # samples"
set key top
set xrange [6000:2000000]
set yrange [0:1]
#set xtics 1
#set ytics 1000
set key top right
set format x "10^{%L}"
#set format y "10^{%L}"
set log x
#set log y
plot "size_pruning_stbs_slow.dat" using 1:2 notitle with points lt 1 lc 3 lw 4 pt 7 ps 4,"size_pruning_stbs_fast.dat" using 1:2 notitle with points lt 1 lc 1 lw 4 pt 9 ps 4

