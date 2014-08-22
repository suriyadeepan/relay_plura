
## A Novel Exploration Approach based on Area of Unexplored region ##
---------------------------------------------------------------------


### Description ###

	1] We find the frontiers by using Edge Detection followed by Contour detection

	2] Considering the ith node, we will find the frontier which offers us best Value Function

			Update Utility Value of each frontier based on its distance to other frontiers which
			 are assigned for other nodes

				Utility = Utility - ( 1 - d/max_range ) if d < max_range
				Utility = Utitlity otherwise

			Find Cost Function  => distance between node and Frontier

			Assign the frontier that offers maximum Value Function = ( Utility - Cost ) as
			destination to our node

	4] Watch it move
	
### Note: ###

	1] This  approach simultaneously takes into account the cost of reaching a target point 
	and its utility

	2] Whenever a target point is assigned to a specific robot, the utility of the unexplored area visible from this target position is reduced. 

	3] Different target locations are assigned to the individual robots.

### Algorithm ###

	#### MAIN: ####	
	- init map
	- while area_explored < 99 do
	- 	for all Nk do
	- 			if(src == destination)
	- 				setBestGridCell()
	-				else
	-					update M with the area covered by the node

	#### SET_BEST_GRIDCELL ####
	- for each pixel M(i,j) do
	- 	D = distance(node[k], pixel(i,j) ) 	
	-		cosine = cos ( node[k] , pixel(i,j) )
	-		sine   = sin ( node[k] , pixel(i,j) )
	-		while d < D
	- 		x = i + ( d * cosine )
	- 		y = i + ( d * sine )
	- 		if ( M(x,y) == BLACK )
	-				utility = utility + 1
	- 	value = utility - ( constant1 * D )
	- 	for each assigned_nodes node[m]
	-			value = value - ( 1 - constant2 * distance(node[m], i, j) )
			if value == MAX_VALUE
				BEST_DEST = pixel(i,j)
	- destination(node[k]) = BEST_DEST
	- draw gray line from BEST_DEST to node[k]
	

### Screenshot ###



