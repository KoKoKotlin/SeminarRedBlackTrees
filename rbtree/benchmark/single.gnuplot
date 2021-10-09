set key autotitle columnhead
unset key
set datafile separator ','

set xlabel "number of nodes"

set key
set key outside

# plot the rb insert time
plot filename using 1:2 with lines, filename using 1:3 with lines, filename using 1:4 with lines, filename using 1:6 with lines, filename using 1:7 with lines, filename using 1:8 with lines
