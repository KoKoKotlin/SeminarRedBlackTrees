set key autotitle columnhead
unset key
set datafile separator ','

set xlabel "number of nodes"
set ylabel "time in ns"

l = 3

set term png font "Monospaced,20.0" size 1700,1500
set output "compare_insert.png"

set key
set key outside

# plot b1, b2 compare
set multiplot layout 3,1
set rmargin 30
set key at screen 1, graph 1

plot "b1.csv" using 1:2 title "insert_{rb} non rec" with lines lw l, "b2.csv" using 1:2 title "insert_{rb} rec" with lines lw l

plot "b1.csv" using 1:3 title "search_{rb} non rec" with lines lw l, "b2.csv" using 1:3 title "search_{rb} rec" with lines lw l

plot "b1.csv" using 1:4 title "delete_{rb} non rec" with lines lw l, "b2.csv" using 1:4 title "delete_{rb} rec" with lines lw l


unset multiplot