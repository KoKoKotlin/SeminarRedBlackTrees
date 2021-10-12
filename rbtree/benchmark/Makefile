FILE = "b1.csv"

benchmark:
	python3 ../benchmark.py

plot1:
	gnuplot -e "filename='${FILE}'" single.gnuplot -p

plot2:
	gnuplot -e "filename='${FILE}'" height.gnuplot -p

plot3:
	gnuplot -p compare.gnuplot
