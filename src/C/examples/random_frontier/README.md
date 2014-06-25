
## Frontier based Exploration ##
--------------------------------

	Frontier is the boundary between the explored and the unexplored regions of a map


### Description ###

	1] We find the frontiers by using Edge Detection followed by Contour detection
	2] Choose 1 frontier for 1 bot in a random fashion ( rand() )
	3] Set Destination of the bot to the selected frontier
	4] Watch it move
	
### Note: ###

		1] Coverage is excellant compared to Random Walk or Waypoint models
		2] There is a one fatal issue : rand() generates the same number at high speeds;
			 Hence, all the nodes follow the same frontiers which makes me really sad.


