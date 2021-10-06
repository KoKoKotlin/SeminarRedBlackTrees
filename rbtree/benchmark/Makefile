FILE = "b.csv"

benchmark:
	python3 ../benchmark.py

plot1:
	gnuplot -e "filename='${FILE}'" single.gnuplot -p

plot2:
	gnuplot -p compare.gnuplot
