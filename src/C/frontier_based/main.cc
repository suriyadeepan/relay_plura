#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "unistd.h"

#include "node.h"
#include "map.h"


double gTime = 0;

	// Get necessary parameters from user 
	//  Map/Grid Dimensions
double	gMax_X = 500;
double	gMax_Y = 500;
	//  #nodes
const int	gN = 3;
	//	Run Time
double	gRunTime = 100000;
	//  Max Speed
double	gMax_Speed = 1;

	// set time resolution
double gT_int = 1;

double gTotalDist;

/* 
 * Frontiers 
 * 	
 * Point	frontiers[200] 
 * 	=> contains coordinates of all frontiers
 *
 * int frontierCount 
 * 	=> num of frontiers
 *
 * int utility[200]
 * 	=> contains utility values of corresponding frontiers
 *
 * int assigned[gN]
 * 	=> contains id of frontiers assigned to nodes
 * 
 * int numAssigned
 * 	=> num of nodes assigned with a frontier
 */
Point frontiers[200];
int frontiersCount = 0;
int utility[200];
int assigned[gN];
int numAssigned = 0;


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
		nodeInit(&ni[i],i,1,15+(i*10));

	// Load Map with nodes
	loadMap(&matrix,ni,gN);

	// Obtain Frontiers
	frontiersCount = getFrontiers(&matrix,frontiers,utility);
	//printf("\nFrontierCount : %d",frontiersCount);

	// Update the map with frontiers
	updateFrontiers(&matrix, frontiers, frontiersCount);

	// Display the initial Map with frontiers
	//imshow("My Map",matrix);

	//waitKey(0);

	clock_t t1,t2;
	// Start clock
	t1 = clock();

	// Start Running till end of gRunTime
	while(gTime <=  gRunTime){

			

			// Iterate through the nodes
		for(int i=0;i<gN;i++){


			// if the nodes has reached the destination (or came kinda close)
			if( calcDist( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY ) < 5){

				// Find the frontier that offers maximum
				//  value function
				int frontierId = assignBestFrontier(&ni[i],frontiers,frontiersCount,
						utility, assigned, numAssigned);

				//printf("frontierId : %d\n",frontierId);

				// Assign the frontier as destination to current node
				if(numAssigned < gN){
					assigned[numAssigned] = frontierId;
					numAssigned++;
				}
				else
					assigned[i] = frontierId;

				// set the next destination of the node as the selected frontier's loca
				ni[i].dstX = frontiers[frontierId].x;
				ni[i].dstY = frontiers[frontierId].y;

				if(i==0)
					gTotalDist += calcDist(ni[i].x,ni[i].y,ni[i].dstX,ni[i].dstY);

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



		}// end of FOR

		/* 
		 * Update the map with current coordinates of nodes
		 * 	FIX IT : add static/mobile flag to struct node
		 *
		 */

		loadMap(&matrix,ni,gN);
		frontiersCount = getFrontiers(&matrix,frontiers,utility);
		updateFrontiers(&matrix, frontiers, frontiersCount);

		//imshow("My Map",matrix);

		// Get Coverage and log it
//		if( (int)gTime % 100 == 0){
			//Snapshot of node locations @ time "gTime"
			//snapshot(&ni[0],gTime);
			coverage = getCoverage(&matrix) * 100;
			/*printf("\n%.0f %.4f",( (double)(clock() - t1)/ 1000000.0F ) * 1000
					,coverage);*/ 
//		}

		char ch = waitKey(5);

		switch(ch){

			case 'q':
				printf("\n\n**** FORCE QUIT by User ****\n");
				return -1;

			case 'p':
				waitKey(0);
		}


		if(coverage > 99.99)
			break;

	 gTime += gT_int;

	}// end of WHILE

	
	//waitKey(1);
	printf("\n\n%.4f\n",gTotalDist);

	return 0;

}
