set key autotitle columnhead
unset key
set datafile separator ','

set xlabel "number of nodes"
set ylabel "height in nodes"
set format x "%.1sE%T"
set xtics font ", 16"

l = 3

set term png font "Monospaced,20.0" size 1700,700
set output "compare_height.png"

set key
set key outside

f(x) = log(x)/log(2)

plot filename using 1:5 with lines lw l, filename using 1:9 with lines lw l, f(x) title "ld(x)" with lines lw l