set key autotitle columnhead
unset key
set datafile separator ','

set ylabel "time in ns"
set xlabel "number of nodes"
set format x "%.1sE%T"
set xtics font ", 16"

set term png font "Monospaced,20.0" size 1700,1500
set output "compare_bin.png"

set key
set key outside

l = 3

# plot the rb insert time
plot filename using 1:2 with lines lw l, filename using 1:3 with lines lw l, filename using 1:4 with lines lw l, filename using 1:6 with lines lw l, filename using 1:7 with lines lw l, filename using 1:8 with lines lw l
