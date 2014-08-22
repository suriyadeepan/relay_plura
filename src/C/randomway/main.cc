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
int	gN = 6;
	//	Run Time
double	gRunTime = 300000;
	//  Max Speed
double	gMax_Speed = 1;

	// set time resolution
double gT_int = 1;
double gTotalDist = 0;

double coverage = 0;

struct node *cluster;
int st_loca_size = 0;

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
	for(int j=0;j<gN;j++)
		nodeInit(&ni[j],j,1,25);

	loadMap(&matrix,ni,gN);


	//imshow("My Map",matrix);

	//waitKey(0);

	// Allocate memory
	/*cluster = (struct node *)malloc(sizeof(struct node)*60);

	int i=0;
	for( int j=0; j< 10; j++)
			nodeRandInit( &cluster[(i*10)+j], (i*10)+j, 51+j, 39+j, 0, 0); 

	i=1;
	for( int j=0; j< 10; j++)
			nodeRandInit( &cluster[(i*10)+j], (i*10)+j, 437+j, 93+j, 0, 0); 

	i=2;
	for( int j=0; j< 10; j++)
			nodeRandInit( &cluster[(i*10)+j], (i*10)+j, 93+j, 429+j, 0, 0); 

	i=3;
	for( int j=0; j< 10; j++)
			nodeRandInit( &cluster[(i*10)+j], (i*10)+j, 343+j, 406+j, 0, 0); 

	i=4;
	for( int j=0; j< 10; j++)
			nodeRandInit( &cluster[(i*10)+j], (i*10)+j, 250+j, 70+j, 0, 0); 

	i=5;
	for( int j=0; j< 10; j++)
			nodeRandInit( &cluster[(i*10)+j], (i*10)+j, 57+j, 250+j, 0, 0); 


	st_loca_size = 60;


//	printf("\nnode id : %d\n",ni[0].node_id);
	*/
	// start clock
	clock_t t1 = clock();

	// Start Running till end of gRunTime
	while(gTime <=  gRunTime){

			// Iterate through the nodes
		for(int i=0;i<gN;i++){

			/*activateCluster(&ni[i], cluster, st_loca_size);
			loadMap(&matrix, cluster, st_loca_size);*/

			// Check if the node has runtime left or not
			if( ni[i].run_time > 0.0 ){

				// if out of bounds
				if(ni[i].x >= gMax_X - 20 || ni[i].y >= gMax_Y - 20 || ni[i].x <= 20 || ni[i].y <= 20 ){

					if ( ni[i].theta < 180 )
						ni[i].theta += 180;
					else
						ni[i].theta = ni[i].theta - 180;
						//ni[i].speed = gMax_Speed/2;
				}

				double offset = ni[i].x;
				ni[i].x = offset + ( ni[i].speed * 0.2 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( ni[i].speed * 0.2 ) * sin(ni[i].theta * 22/ (7*180) ) ;

				ni[i].run_time -= 0.2;

					if(i==0)
						gTotalDist += 0.2;

					coverage = getCoverage(&matrix) *100;
					printf("\n%.0f %.4f",gTotalDist, coverage);


			}// end of RUNTIME_LEFT condition

			// No runtime left
			else{

				// Rest/Pause time left
				if( ni[i].rest_time > 0 )
					ni[i].rest_time -= 0.2;

				// No Rest/Pause time left
				else{

					srand(clock()+i);
					double c_speed = ( (double)rand()/(double)RAND_MAX ) * gMax_Speed;
/*					move_rwy ( &ni[i],( (double)rand()/(double)RAND_MAX ) * gMax_X,
								 				( (double)rand()/(double)RAND_MAX ) * gMax_Y,
												c_speed,	
												( (double)rand()/(double)RAND_MAX ) * gMax_Pause );
												*/
				move_rwy ( &ni[i], ( (double)rand()/(double)RAND_MAX ) * gMax_X,
								 				( (double)rand()/(double)RAND_MAX ) * gMax_Y,
												1,0);


					
				}// end of NO_PAUSETIME_LEFT condition


			}// end of NO_RUNTIME_LEFT condition


		}// end of FOR

		/* 
		 * Update the map with current coordinates of nodes
		 * 	FIX IT : add static/mobile flag to struct node
		 *
		 */

		loadMap(&matrix,ni,gN);
		//imshow("My Map",matrix);

		/*char ch = waitKey(3);

		switch(ch){

			case 'q':
				//printf("\n\n***FORCE QUIT BY User***\n");
				return -1;

			case 'p':
				waitKey(0);
		}*/

 	 //Snapshot of node locations @ time "gTime"
	 //snapshot(&ni[0],gTime);
/*	 double coverage = getCoverage(&matrix)*100;
	 printf("\n%.0f %.4f",( (double)(clock() - t1)/ 1000000.0F ) * 1000
					,coverage); 
					*/

	 if(coverage > 99)
		 break;

	 gTime += gT_int;


	}// end of WHILE

	
	//waitKey(1);

	return 0;

}
