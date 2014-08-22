
## Frontier based Exploration with Utility vs Cost Approach ##
--------------------------------

	Frontier is the boundary between the explored and the unexplored regions of a map


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

### Screenshot ###


