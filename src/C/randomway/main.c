#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

#include "node.h"

double gTime = 0;

int main(){

	// Get necessary parameters from user 
	//  Map/Grid Dimensions
	gMax_X = 100;
	gMax_Y = 100;
	//  #nodes
	gN = 5;
	//	Run Time
	gRunTime = 10000;
	//  Max Speed
	gMax_Speed = 10;
	//  Max Pause Time
	gMax_Pause = 2;

	int i;

	// Pointer to nodes
	struct node* ni;

	ni =(struct node *)malloc(sizeof(struct node)*gN);

	/*
	 * Initialize Nodes
	 */
	for(i=0;i<gN;i++)
		nodeInit(&ni[i],i);

//	printf("\nnode id : %d\n",ni[0].node_id);


	// Start Running till end of gRunTime
	while(gTime <=  gRunTime){

		// Iterate through the nodes
		for(i=0;i<gN;i++){

			// if out of bounds
			/*
			if(ni[i].x >= gMax_X || ni[i].y >= gMax_Y){
				ni[i].rest_time = 0.0;
				ni[i].run_time = 0.0;
			}*/


			// Check if the node has runtime left or not
			if( ni[i].run_time > 0.0 ){

				if(ni[i].x >= gMax_X || ni[i].y >= gMax_Y || ni[i].x <= 0 || ni[i].y <= 0 || ni[i].dstX >= gMax_X || ni[i].dstY >= gMax_Y || ni[i].dstX <= 0 || ni[i].dstY <= 0 ){
	
					srand(clock()+i);

					move ( &ni[i],( (double)rand()/(double)RAND_MAX ) * gMax_X,
								 				( (double)rand()/(double)RAND_MAX ) * gMax_Y,
												( (double)rand()/(double)RAND_MAX ) * gMax_Speed,
												( (double)rand()/(double)RAND_MAX ) * gMax_Pause );
				}

				else{

					double offset = ni[i].x;
					ni[i].x = offset + ( ni[i].speed * 0.2 ) * cos(ni[i].theta) ;

					offset = ni[i].y;
					ni[i].y = offset + ( ni[i].speed * 0.2 ) * sin(ni[i].theta) ;

					ni[i].run_time -= 0.2;
				}

			}// end of RUNTIME_LEFT condition

			// No runtime left
			else{

				// Rest/Pause time left
				if( ni[i].rest_time > 0 )
					ni[i].rest_time -= 0.2;

				// No Rest/Pause time left
				else{

					srand(clock()+i);

					move ( &ni[i],( (double)rand()/(double)RAND_MAX ) * gMax_X,
								 				( (double)rand()/(double)RAND_MAX ) * gMax_Y,
												( (double)rand()/(double)RAND_MAX ) * gMax_Speed,
												( (double)rand()/(double)RAND_MAX ) * gMax_Pause );
					

				}// end of NO_PAUSETIME_LEFT condition


			}// end of NO_RUNTIME_LEFT condition



		}// end of FOR

 	 //Snapshot of node locations @ time "gTime"
	 snapshot(&ni[0],gTime);

	 gTime += 0.2;

	}// end of WHILE

	return 0;

}
