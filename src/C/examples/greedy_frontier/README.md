
## Frontier based Exploration with Greedy Approach ##
--------------------------------

	Frontier is the boundary between the explored and the unexplored regions of a map


### Description ###

	1] We find the frontiers by using Edge Detection followed by Contour detection
	2] Considering the ith node, we will find the frontier closest to its position
	3] Set Destination of the bot to the selected frontier
	4] Watch it move
	
### Note: ###

	1] Greedy algorithm only considers interesting locations locally
	2] It is focused to reduce the movement costs. 
	3] More sophisticated algorithms also take into account the information that
  	 can be gained along each frontier.

### Screenshot ###

![picture alt]( https://raw.githubusercontent.com/utortuga/relay_plura/master/screenshots/frontier_based_greedy.png  "Greedy Frontier GNUPlot" )

