#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "unistd.h"

#include "node.h"
#include "map.h"

// comm traffic
int traffic = 0;

double gTime = 0;

// Get necessary parameters from user 
//  Map/Grid Dimensions
double	gMax_X = 500;
double	gMax_Y = 500;
//  #nodes
int	gN ;
//	Run Time
double	gRunTime = 100000;
//  Max Speed
double	gMax_Speed = 1;

// set time resolution
double gT_int = 1;

double gTotalDist = 0.0;

// #(discovered static nodes)
int gDiscNodes = 0;

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
int assigned[10];
int numAssigned = 0;


double coverage = 0.0;

/*  Clusters **/
// Pointers to clusters 
struct node *cluster;

// Static node locations
Point st_loca[40];
int st_loca_size=0;

/*void CallBackFunc(int event, int x, int y, int flags, void* userdata)
	{
	if  ( event == EVENT_LBUTTONDOWN )
	{
	printf("\n(%d,%d)",x,y);
	st_loca[st_loca_size].x = x;
	st_loca[st_loca_size++].y = y;
	}
	}*/



int main(int argc,char** argv){


	int i=0;

	gN = atoi(argv[1]);

	Mat matrix;

	// Initialize Map
	initMap(&matrix,gMax_X,gMax_Y);


	// Pointer to mobile nodes
	struct node* ni;

	// Allocate memory for mobile nodes
	ni =(struct node *)malloc(sizeof(struct node)*gN);

	/*
	 * Initialize Mobile Nodes
	 */
	for(i=0;i<gN;i++)
		nodeInit(&ni[i],i,1,25);

	// Load Map with nodes
	loadMap(&matrix,ni,gN);


	// Obtain Frontiers
	frontiersCount = getFrontiers(&matrix,frontiers,utility);
	//printf("\nFrontierCount : %d",frontiersCount);

	// Update the map with frontiers
	updateFrontiers(&matrix, frontiers, frontiersCount);

	// Display the initial Map with frontiers
	imshow("My Map",matrix);

	//set the callback function for any mouse event
	//  setMouseCallback("My Map", CallBackFunc, NULL);

	waitKey(0);

	// Allocate memory
	cluster = (struct node *)malloc(sizeof(struct node)*140);


	int j=0;
	for( j=0; j< 33; j++)
		nodeRandInit( &cluster[j], j, 51+j, 39+j, 0, 0); 

	for( ; j< 53; j++)
		nodeRandInit( &cluster[j], j, 437+j, 93+j, 0, 0); 

	for( ; j< 66; j++)
		nodeRandInit( &cluster[j], j, 93+j, 429+j, 0, 0); 

	for( ; j< 88; j++)
		nodeRandInit( &cluster[j], j, 343+j, 406+j, 0, 0); 

	for( ; j< 97; j++)
		nodeRandInit( &cluster[j], j, 250+j, 70+j, 0, 0); 

	for( ; j< 117; j++)
		nodeRandInit( &cluster[j], j, 57+j, 250+j, 0, 0); 

	for( ; j< 130; j++)
		nodeRandInit( &cluster[j], j, 418, 290, 0, 0); 

	for( ; j< 140; j++)
		nodeRandInit( &cluster[j], j, 0,0,0,0); 


	st_loca_size = 140;


	// add to map
	loadMap(&matrix,cluster,st_loca_size);
	overlayNodes(&matrix, cluster, st_loca_size);

	waitKey(0);

	clock_t t1,t2;
	// Start clock
	t1 = clock();

	//	printf("\n%.0f %.4f", gTotalDist, getCoverage(&matrix)*100 ); 
	printf("\n%.0f %d", gTotalDist, gDiscNodes);

	// start clock
	//clock_t t1 = clock();

	for(i=0;i<gN;i++){
		srand( clock() );
		move_rwk ( &ni[i], 1, ( (double)rand()/(double)RAND_MAX )*360 );
	}


	// Start Running till end of gRunTime
	while(gDiscNodes <= 135*0.8){

		// Iterate through the nodes
		for(i=0;i<gN;i++){

			activateCluster(&ni[i], cluster, st_loca_size);

			loadMap(&matrix,cluster,st_loca_size);
			overlayNodes(&matrix, cluster, st_loca_size);

			// if out of bounds
			if(ni[i].x >= gMax_X - 10 || ni[i].y >= gMax_Y - 10 || ni[i].x <= 10 || ni[i].y <= 10 ){

				if ( ni[i].theta < 180 )
					ni[i].theta += 180;

				else
					ni[i].theta = ni[i].theta - 180;
				//ni[i].speed = gMax_Speed/2;

				double offset = ni[i].x;
				ni[i].x = offset + ( ni[i].speed * 4 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( ni[i].speed * 4 ) * sin(ni[i].theta * 22/ (7*180) ) ;



				srand( clock() );
				move_rwk ( &ni[i], 1, ( (double)rand()/(double)RAND_MAX )*360 );

				traffic++;
				//printf("\n\nRD Traffic : %d\n",traffic);
			}

			else{

				double offset = ni[i].x;
				ni[i].x = offset + ( ni[i].speed * 1 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( ni[i].speed * 1 ) * sin(ni[i].theta * 22/ (7*180) ) ;


				if(i==2)
					gTotalDist += 1;

				coverage = getCoverage(&matrix) *100;
				printf("\n%.0f %d",gTotalDist, gDiscNodes);

			}
		}// end of FOR

		/* 
		 * Update the map with current coordinates of nodes
		 * 	FIX IT : add static/mobile flag to struct node
		 *
		 */

		loadMap(&matrix,ni,gN);
		overlayNodes(&matrix, cluster, st_loca_size);

		imshow("My Map", matrix);
		waitKey(2);

	}// end of WHILE

	for(int k=0; k<gN; k++){
		ni[k].dstX = ni[k].x; 		ni[k].dstY = ni[k].y; 
	}

	//printf("\n\nRD Traffic : %d\n",traffic);
	waitKey(0);


	// Start Running till end of gRunTime
	while(gDiscNodes <= 135){
		// Iterate through the nodes
		for(i=0;i<gN;i++){

			activateCluster(&ni[i], cluster, st_loca_size);

			loadMap(&matrix,cluster,st_loca_size);
			overlayNodes(&matrix, cluster, st_loca_size);

			// if the nodes has reached the destination (or came kinda close)
			if( calcDist( ni[i].x ,ni[i].y ,ni[i].dstX ,ni[i].dstY ) < 5){

				// Find the frontier that offers maximum
				//  value function
				int frontierId = assignBestFrontier(&ni[i],frontiers,frontiersCount,
						utility, assigned, numAssigned);


				// Assign the frontier as destination to current node
				if(numAssigned < gN){
					assigned[numAssigned] = frontierId;
					numAssigned++;
				}
				else
					assigned[i] = frontierId;

				traffic++;

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

				if(i==0)
					gTotalDist += gMax_Speed;

				coverage = getCoverage(&matrix) * 100;
				//printf("\n%.0f %.4f", gTotalDist, coverage); 
				printf("\n%.0f %d", gTotalDist, gDiscNodes); 


			}



		}// end of FOR

		/* 
		 * Update the map with current coordinates of nodes
		 * 	FIX IT : add static/mobile flag to struct node
		 *
		 */

		loadMap(&matrix,ni,gN);
		overlayNodes(&matrix, cluster, st_loca_size);
		frontiersCount = getFrontiers(&matrix,frontiers,utility);
		updateFrontiers(&matrix, frontiers, frontiersCount);

		imshow("My Map",matrix);

		// Get Coverage and log it
		//		if( (int)gTime % 100 == 0){
		//Snapshot of node locations @ time "gTime"
		//snapshot(&ni[0],gTime);
		//		}

		char ch = waitKey(5);

		switch(ch){

			case 'q':
				//printf("\n\n**** FORCE QUIT by User ****\n");
				return -1;

			case 'p':
				waitKey(0);
		}

		if(gDiscNodes >= 134)
			break;


	}


	//printf("\n\n Traffic : %d\n",traffic);

	return 0;

}
