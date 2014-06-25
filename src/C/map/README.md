## Map Builder ##
-----------------

	Create, Update and Log node positions.

### Description ###

	map.h consists of functions that help us create, update and maintain 
  a map of the whole area to be explored. It keeps track of available
	mobile and static nodes in the area. The area explored and unexplored 
	are differentiated by colors. Static and Mobile nodes are also 
  distinguished by colors. OpenCV's Mat structure is used for this
	purpose. As the location of nodes are updated, we can witness the 
	changes in the map displayed in a window. The Coverage of area w.r.t
	time is logged and plotted using gnuplot.

### Compilation ###

	$ make

### Plotting ###

	$ ./main.bin > coverage_plot.dat

	# In GNUPLOT
	$ gnuplot> plot "coverage_plot.dat" using 1:2

### Screenshots ###

![picture alt]( https://raw.githubusercontent.com/utortuga/relay_plura/master/screenshots/RandomWalk_Sim_opencv00.png "Random Walk Simulation using OpenCV")

![picture alt]( https://raw.githubusercontent.com/utortuga/relay_plura/master/screenshots/RandomWalk_GNUPLOT.png  "Random Walk Coverage GNUPlot")

