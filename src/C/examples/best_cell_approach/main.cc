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


double coverage = 0.0;

int main(int argc,char** argv){

	Mat matrix,bw;

	// Initialize Map
	initMap(&matrix,gMax_Y,gMax_Y);

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
	loadMap(&matrix,&bw,ni,gN);


	// Display the initial Map 
	imshow("My Map",matrix);

	// *Pause
	waitKey(0);

	// Start clock
	clock_t t1;
	t1 = clock();


	//printf("\n\n **** Before Loop ****\n\n");
	// Start Running till end of gRunTime
	while(gTime <=  gRunTime){

			

			// Iterate through the nodes
		for(int i=0;i<gN;i++){

			//printf("\n\n **** Node %d****\n\n",ni[i].node_id);

			// if the nodes has reached the destination (or came kinda close)
			if( calcDist( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY ) < 5){

				// Find the cell that offers maximum
				//  value function
				assignBestCell(&ni[i],&matrix);


				if(i==0)
					gTotalDist += calcDist(ni[i].x,ni[i].y,ni[i].dstX,ni[i].dstY);

				// update the direction/angle (theta)
				ni[i].theta = calcTheta( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY );

				printf("\n Node %d : (%.2f,%.2f) --> (%.2f,%.2f)",ni[i].node_id, ni[i].x,
						 ni[i].y, ni[i].dstX, ni[i].dstY);

			}

			/*	Else
			 *	 Keep Movin!
			 */
			else{

				printf("\nKeep Moving\n");

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

		loadMap(&matrix,&bw,ni,gN);

		imshow("My Map",matrix);

		// Get Coverage and log it
		if( (int)gTime % 100 == 0){
			//Snapshot of node locations @ time "gTime"
			snapshot(&ni[0],gTime);
			//coverage = getCoverage(&matrix) * 100;
			/*printf("\n%.0f %.4f",( (double)(clock() - t1)/ 1000000.0F ) * 1000
					,coverage);*/ 
		}

		char ch = waitKey(10);

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
