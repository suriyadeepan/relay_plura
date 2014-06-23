#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "node.h"

int main(){

	// Get necessary parameters from user 
	//  Map/Grid Dimensions
	gMax_X = 300;
	gMax_Y = 300;
	//  #nodes
	gN = 5;
	//	Run Time
	gRunTime = 1000;
	//  Max Spped
	gMax_Speed = 5;

	int i;

	// Pointer to nodes
	struct node* ni;

	ni =(struct node *)malloc(sizeof(struct node)*gN);

	/*
	 * Initialize Nodes
	 */
	for(i=0;i<gN;i++)
		nodeInit(&ni[i],i+1);

	 //Snapshot of nodes
	snapshot(&ni[0]);

//	printf("\nnode id : %d\n",ni[0].node_id);


	// Start Running till end of gRunTime
	while(--gRunTime > 0){

		// Iterate through the nodes
		for(i=0;i<gN;i++){
			
			// Check if the node has runtime left or not
			if( ni[i].run_time > 0){


				ni[i].run_time -= 0.2;
			}// end of RUNTIME_LEFT condition

			// No runtime left
			else{

				// Rest/Pause time left
				if( ni[i].rest_time > 0 )
					ni[i].rest_time -= 0.2;

				// No Rest/Pause time left
				else{

					srand(clock());

					move ( &ni[i],( (double)rand()/(double)RAND_MAX ) * gMax_X,
								 				( (double)rand()/(double)RAND_MAX ) * gMax_Y,
												( (double)rand()/(double)RAND_MAX ) * gMax_Speed );
					

				}// end of NO_PAUSETIME_LEFT condition


			}// end of NO_RUNTIME_LEFT condition



		}// end of FOR



	}// end of WHILE

	return 0;

}
