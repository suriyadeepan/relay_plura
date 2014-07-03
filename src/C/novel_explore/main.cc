#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "unistd.h"

#include "node.h"
#include "map.h"


// global bw matrix
Mat bwMat;

double gTime = 0;

// Get necessary parameters from user 
//  Map/Grid Dimensions
double	gMax_X = 500;
double	gMax_Y = 500;

//  #nodes
const int	gN = 4;
//	Run Time
double	gRunTime = 100000;
//  Max Speed
double	gMax_Speed = 1;

// set time resolution
double gT_int = 1;

double gTotalDist;

double gC = 0.0;


/*
 * Keep track of assigned nodes
 *  add a factor ( distance to assigned nodes )
 *   to the value function
 *    makes the nodes spread 
 */
Point assigned[gN];
int numAssigned = 0;



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
		nodeInit(&ni[i],i,1,40);

	// Load Map with nodes
	loadMap(&matrix,ni,gN);

	// Display the initial Map with frontiers

	imshow("My Map",matrix);

	waitKey(0);

	clock_t t1,t2;
	// Start clock
	t1 = clock();

	// Start Running till end of gRunTime
	while(gTime <=  gRunTime){

		// Iterate through the nodes
		for(int i=0;i<gN;i++){


			// if the nodes has reached the destination (or came kinda close)
			if( calcDist( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY ) < 30){

				// Find the destination with highest utility value
				//  and set it as destination for the node
				loadMap(&matrix,ni,gN);
				setBestDestination(&ni[i],&matrix, assigned, numAssigned);

				if(numAssigned < gN){

					assigned[numAssigned] = Point(ni[i].dstX, ni[i].dstY);
					numAssigned++;

				}
				else
					assigned[i] = Point(ni[i].dstX, ni[i].dstY);


				// update the direction/angle (theta)
				ni[i].theta = calcTheta( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY );


			}

			/*	Else
			 *	 Keep Movin!
			 */
			else{

				//setBestDestination(&ni[i],&matrix);
				loadMap(&matrix,ni,gN);
				imshow("bw",bwMat);

				// Speed is constant
				double offset = ni[i].x;

				ni[i].x = offset + ( gMax_Speed * 1 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( gMax_Speed * 1 ) * sin(ni[i].theta * 22/ (7*180) ) ;

				// keep track of total distance travelled by node0
				if(i==0){
						gTotalDist += gMax_Speed;
						gC = getCoverage(&matrix) * 100;
						printf("\n%.4f %.4f",gTotalDist, gC); 
				}




			}



		}// end of FOR

		/* 
		 * Update the map with current coordinates of nodes
		 * 	FIX IT : add static/mobile flag to struct node
		 *
		 */

		loadMap(&matrix,ni,gN);

		imshow("My Map",matrix);

		// Get Coverage and log it
		//		if( (int)gTime % 100 == 0){
		//Snapshot of node locations @ time "gTime"
		//snapshot(&ni[0],gTime);
		//		}

		char ch = waitKey(5);

		switch(ch){

			case 'q':
				printf("\n\n**** FORCE QUIT by User ****\n");
				return -1;

			case 'p':
				waitKey(0);

			case 'c':
				printf("\nCoverage : %.3f",gC);
		}


		if(gC > 99.9)
			break;

		gTime += gT_int;

	}// end of WHILE


	waitKey(1);
	//printf("\n\n%.4f\n",gTotalDist);

	return 0;

}
