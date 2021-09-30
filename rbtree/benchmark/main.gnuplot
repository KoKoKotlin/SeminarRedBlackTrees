set datafile separator ','
set xdata time
set timefmt "%Y-%m-%dT%H:%M:%S"
set key autotitle columnhead
set ylabel "First Y Units" 
set xlabel 'Time'

set y2tics # enable second axis
set ytics nomirror # dont show the tics on that side
set y2label "Second Y Axis Value" # label for second axis

plot b.csv using 1:2 with lines, '' using 1:3 with lines, '' using 1:4 with lines axis x1y2