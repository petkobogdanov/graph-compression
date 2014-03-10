set term postscript eps 35 enhanced color solid
set style fill solid 1.00 border
set encoding iso_8859_1
set output "avg_std_compression_time_radius.eps"
set xlabel "radius"
set ylabel "time (sec.)"
set key top
set xrange [0.5:3.5]
set yrange [0:2400]
set xtics 1
set ytics 500
set key top right
plot "avg_std_compression_time_st.dat" using 1:2 title 'ST' with linespoints lt 1 lc 1 lw 4 pt 2 ps 4, "avg_std_compression_time_stus.dat" using 1:2 title 'STUS' with linespoints lt 1 lc 2 lw 4 pt 5 ps 4,"avg_std_compression_time_stbs.dat" using 1:2 title 'STIS' with linespoints lt 1 lc 3 lw 4 pt 7 ps 4

