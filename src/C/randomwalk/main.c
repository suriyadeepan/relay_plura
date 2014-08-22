#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

#include "node.h"

double gTime = 0;

int main(){

	// Get necessary parameters from user 
	//  Map/Grid Dimensions
	gMax_X = 250;
	gMax_Y = 250;
	//  #nodes
	gN = 6;
	//	Run Time
	gRunTime = 300000;
	//  Max Speed
	gMax_Speed = 4;

	// set time resolution
	gT_int = 0.2;

	// set time interval
	double t_int = 1;

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

			// every 't_int' time units
			//  set new speed, new direction
			if( fmod(gTime,t_int) == 0){

				// set speed, theta
				srand(clock() + i);
				move_rwk( &ni[i], ( (double)rand()/(double)RAND_MAX ) * gMax_Speed, 
			  									( (double)rand()/(double)RAND_MAX ) * 360 ); 

			}

		  else{

				// find new location of x,y based on (speed) distance and angle
				update_rwk(&ni[i]);

			}

				// if out of bounds
			if(ni[i].x >= gMax_X - 20 || ni[i].y >= gMax_Y - 20 || ni[i].x <= 20 || ni[i].y <= 20 ){

				if ( ni[i].theta < 180 )
					ni[i].theta += 180;
				else
					ni[i].theta = ni[i].theta - 180;
					//ni[i].speed = gMax_Speed;
			}


		}// end of FOR

 	 //Snapshot of node locations @ time "gTime"
	 snapshot(&ni[0],gTime);

	 gTime += gT_int;

	}// end of WHILE

	return 0;

}
