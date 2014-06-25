#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

#include "node.h"
#include "map.h"


double gTime = 0;

	// Get necessary parameters from user 
	//  Map/Grid Dimensions
double	gMax_X = 500;
double	gMax_Y = 500;
	//  #nodes
double	gN = 5;
	//	Run Time
double	gRunTime = 100000;
	//  Max Speed
double	gMax_Speed = 1;

	// set time resolution
double gT_int = 1;


/* Frontiers */
Point frontiers[200];
int frontiersCount = 0;

double coverage = 0.0;

int main(int argc,char** argv){

	Mat matrix;

	// Initialize Map
	initMap(&matrix,gMax_X,gMax_Y);

	// Pointer to nodes
	struct node* ni;

	// Allocate memory for nodes
	ni =(struct node *)malloc(sizeof(struct node)*gN);

	/*
	 * Initialize Nodes
	 */
	for(int i=0;i<gN;i++)
		nodeInit(&ni[i],i);

	// Load Map with nodes
	loadMap(&matrix,ni,gN);

	// Obtain Frontiers
	frontiersCount = getFrontiers(&matrix,frontiers);

	// Update the map with frontiers
	updateFrontiers(&matrix, frontiers, frontiersCount);

	// Display the initial Map with frontiers
	imshow("My Map",matrix);

	waitKey(0);

	//srand(time(NULL));
	srand(static_cast<int>(time(0)));

	// Start Running till end of gRunTime
	while(gTime <=  gRunTime){

			

			// Iterate through the nodes
		for(int i=0;i<gN;i++){


			// if the nodes has reached the destination (or came kinda close)
			if( calcDist( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY ) < 5){

				// Find a random frontier to approach next
				int frontierId = (int) ( (double)rand()/(double)RAND_MAX ) * frontiersCount; 

				// set the next destination of the node as the selected frontier's loca
				ni[i].dstX = frontiers[frontierId].x;
				ni[i].dstY = frontiers[frontierId].y;

				// update the direction/angle (theta)
				ni[i].theta = calcTheta( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY );

			}

			/*	Else
			 *	 Keep Movin!
			 */
			else{

				// Speed is constant
				double offset = ni[i].x;

				ni[i].x = offset + ( gMax_Speed * 1 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( gMax_Speed * 1 ) * sin(ni[i].theta * 22/ (7*180) ) ;

			}


				// if out of bounds
				//  we need Reflecting boundaries
				//   *** DO WE NOW?? ***
			if(ni[i].x >= gMax_X - 20 || ni[i].y >= gMax_Y - 20 || ni[i].x <= 20 || ni[i].y <= 20 ){

				// Find a random frontier to approach next
				int frontierId = (int) ( (double)rand()/(double)RAND_MAX ) * frontiersCount; 

				// set the next destination of the node as the selected frontier's loca
				ni[i].dstX = frontiers[frontierId].x;
				ni[i].dstY = frontiers[frontierId].y;

				// update the direction/angle (theta)
				ni[i].theta = calcTheta( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY );

			}


		}// end of FOR

		/* 
		 * Update the map with current coordinates of nodes
		 * 	FIX IT : add static/mobile flag to struct node
		 *
		 */

		loadMap(&matrix,ni,gN);
		frontiersCount = getFrontiers(&matrix,frontiers);
		updateFrontiers(&matrix, frontiers, frontiersCount);

		imshow("My Map",matrix);

		// Get Coverage and log it
		if( (int)gTime % 100 == 0){
			//Snapshot of node locations @ time "gTime"
			//snapshot(&ni[0],gTime);
			srand(static_cast<int>(time(0)));
			//srand(time(NULL));
			//printf("\n%.0f %.4f", gTime, getCoverage(&matrix) * 100 );
			coverage = getCoverage(&matrix) * 100;
		}

		waitKey(1);

		if(coverage > 99)
			break;

	 gTime += gT_int;

	}// end of WHILE

	
	waitKey(1);

	return 0;

}
