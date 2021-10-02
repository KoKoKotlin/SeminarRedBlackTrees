set key autotitle columnhead
set datafile separator ','

set xlabel "number of nodes"

# plot the rb insert time
plot "b.csv" using 1:2 with lines, "b.csv" using 1:3 with lines, "b.csv" using 1:4 with lines, "b.csv" using 1:6 with lines, "b.csv" using 1:7 with lines, "b.csv" using 1:8 with lines