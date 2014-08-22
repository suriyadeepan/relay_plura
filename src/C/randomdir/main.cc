#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

#include "node.h"
#include "map.h"


double gTime = 0;

double gMax_Pause;

// Get necessary parameters from user 
//  Map/Grid Dimensions
double	gMax_X = 500;
double	gMax_Y = 500;
//  #nodes
int	gN = 4;
//	Run Time
double	gRunTime = 100000;
//  Max Speed
double	gMax_Speed = 1;

// set time resolution
double gT_int = 1;
double gTotalDist = 0;

double coverage = 0;

struct node *cluster;
int st_loca_size = 0;

// #(discovered nodes)
int gDiscNodes = 0;

// comm traffic
int traffic = 0;

int main(){

	Mat matrix;

	initMap(&matrix,gMax_X,gMax_Y);

	// set time interval
	int t_int = 4;


	// Pointer to nodes
	struct node* ni;

	ni =(struct node *)malloc(sizeof(struct node)*gN);

	/*
	 * Initialize Nodes
	 */
	for(int j=0;j<gN;j++){
		nodeInit(&ni[j],j,1,25);
	}


	loadMap(&matrix,ni,gN);


	imshow("My Map",matrix);

	waitKey(0);

	// Allocate memory
	/*cluster = (struct node *)malloc(sizeof(struct node)*140);

	int i=0;
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

		*/

	// start clock
	clock_t t1 = clock();

	for(int i=0;i<gN;i++){
		srand( clock() );
		move_rwk ( &ni[i], 1, ( (double)rand()/(double)RAND_MAX )*360 );
	}
	

	// Start Running till end of gRunTime
//	while(gDiscNodes <= (0.95 * 140) ){
		while(coverage < 98 ){

				// Iterate through the nodes
		for(int i=0;i<gN;i++){

		//	activateCluster(&ni[i], cluster, st_loca_size);
		//	loadMap(&matrix, cluster, st_loca_size);

			// if out of bounds
			if(ni[i].x >= gMax_X - 20 || ni[i].y >= gMax_Y - 20 || ni[i].x <= 20 || ni[i].y <= 20 ){

			/*	printf("\nhere");
				if ( ni[i].theta < 180 )
					ni[i].theta += 180;
				else
					ni[i].theta = ni[i].theta - 180;
					*/

				srand( clock() );
				move_rwk ( &ni[i], 1, ( (double)rand()/(double)RAND_MAX )*360 );
				traffic++;
	
				double offset = ni[i].x;
				ni[i].x = offset + ( ni[i].speed * 1 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( ni[i].speed * 1 ) * sin(ni[i].theta * 22/ (7*180) ) ;

			}

			else{

				double offset = ni[i].x;
				ni[i].x = offset + ( ni[i].speed * 1 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( ni[i].speed * 1 ) * sin(ni[i].theta * 22/ (7*180) ) ;


				if(i==2)
				gTotalDist += 1;

				coverage = getCoverage(&matrix) *100;
				printf("\n%.0f %.4f",gTotalDist, coverage);
				//printf("\n%.0f %d",gTotalDist, gDiscNodes);
				//printf("\n%.0f %d",gTotalDist, traffic);
			}



		}// end of FOR

		/* 
		 * Update the map with current coordinates of nodes
		 * 	FIX IT : add static/mobile flag to struct node
		 *
		 */

		loadMap(&matrix,ni,gN);
		//overlayNodes(&matrix, cluster, st_loca_size);
		imshow("My Map",matrix);

		char ch = waitKey(3);

			switch(ch){

			case 'q':
			//printf("\n\n***FORCE QUIT BY User***\n");
			return -1;

			case 'p':
			waitKey(0);
			}

		//Snapshot of node locations @ time "gTime"
		//snapshot(&ni[0],gTime);
		/*	 double coverage = getCoverage(&matrix)*100;
				 printf("\n%.0f %.4f",( (double)(clock() - t1)/ 1000000.0F ) * 1000
				 ,coverage); 
				 */

/*		if(coverage > 70)
			break;
			*/

		gTime += gT_int;


	}// end of WHILE


	//waitKey(1);

	return 0;

}
