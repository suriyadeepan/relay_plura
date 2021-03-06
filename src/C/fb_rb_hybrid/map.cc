#include "map.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "stdio.h"

/*
 * Constructors
 * 	1] Initialize Map with sizeX and size Y
 * 	2] Initialzie Map with dimensions, nodes
 */

int initMap (Mat* matrix, int sizeX, int sizeY){

	// Inilialize matrix
	*matrix = Mat(sizeX, sizeY, CV_8UC3);

	return 0;

}

void overlayNodes(Mat *matrix, struct node *cluster, int st_loca_size){

	for( int i=0; i< st_loca_size; i++ )
		circle( *matrix, Point(cluster[i].x,cluster[i].y), 2 , Scalar(0,0,255), -1, 8, 0 );

}


/*
 * Call mapInitAll() along with loadMap()
 */

//int mapInitAll (struct map *,int, int, struct node *, int);


/*
 * Load node locations into map
 */
int loadMap(Mat* matrix,struct node *n0, int nodeCount){

	int x,y;
	/*
	 * Iterate through the node locations till nodeCount
	 * 	Draw circles at those locations in the map
	 */
	for(int i=0; i<nodeCount; i++){

		x = (int) *(&n0[i].x);
		y = (int) *(&n0[i].y);


		if(n0[i].pl > 0)
		circle( *matrix, Point(x,y), (int)*(&n0[i].pl), Scalar(255,255,255), -1, 8, 0 );

		else
		circle( *matrix, Point(x,y), 40, Scalar(30,30,30), -1, 8, 0 );

		if( (int)*(&n0[i].type) == 1 )
			circle( *matrix, Point(x,y), 5 , Scalar(0,255,0), -1, 8, 0 );

		/*else
			circle( *matrix, Point(x,y), 5 , Scalar(0,0,255), -1, 8, 0 );
			*/


	}// END OF FOR



	return 0;

}

void updateFrontiers(Mat *matrix,Point *frontiers,int frontierCount){

	Mat frontierMap;
 	(*matrix).copyTo(frontierMap);

	/*
	 * Update the map with frontiers
	 */
	for(int i=0; i<frontierCount; i++){

		circle( frontierMap, frontiers[i], 2, Scalar(255,0,0), -1, 8, 0 );

	}

	//imshow("Frontier Map",frontierMap);

}


/*
 * Not interested in plotting for now
 */
double getCoverage(Mat* matrix ){


	/* Calculate Coverage ( ratio of white to black )
	 *
	 * 	1] convert *matrix to GrayScale
	 * 	2] Binary threshold GrayScale image
	 * 	3] call countNonZero() method
	 */
	Mat dst;
	cvtColor(*matrix,dst,CV_BGR2GRAY,0);
	threshold(dst,dst,50,255,THRESH_BINARY);

	//imshow("Cov",dst);

	double coverage = (double)( countNonZero(dst)/(gMax_X*gMax_Y) );

	// deallocate memory
	dst.release();

	return coverage;

}

int getFrontiers(Mat *matrix, Point *frontiers,int *utility){

	Mat cannied;
	vector<vector<Point> > contours;

	// convert BGR to GRAYSCALE
	cvtColor(*matrix,cannied,CV_BGR2GRAY,0);

	// convert GRAYSCALE to binary
	threshold(cannied,cannied,50,255,THRESH_BINARY);

	// Apply Canny filter for Edge Detection
	Canny(cannied, cannied , 50, 240, 3, false);

	// Find contours
	findContours( cannied, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	// CV_RETR_LIST


	int k=0;
	// print all the pixel coordinates in the contours
	for(size_t i=0; i<contours.size(); i++ ){
		   // use contours[i] for the current contour
			 for(size_t j=0; j<contours[i].size(); j++ ){
			           // use contours[i][j] for current point
								 if(j%20 == 0){
									 //printf("\n%d %d",contours[i][j].x,contours[i][j].y);
									 frontiers[k] = contours[i][j];
									 utility[k] = 1;
									 k++;
								 }
			 }
	}

	return k;

}


/*
 * Find the best possible frontier to explore for a particular node
 *  based on Value Function = ( Utility - Cost )
 *
 * Assign it to the node
 */
int assignBestFrontier(struct node *n0, Point *frontiers, int frontierCount,int *utility,
	 	int *assigned, int numAssigned){

	int frontierId = 0;
	int maxVal = -10000;

	// Iterate through the frontiers
	for(int i=0; i<frontierCount; i++){

		/*
		 * To calculate Utility,
		 * 	iterate through the assigned frontiers array
		 *
		 * Update Utility
		 */
		for(int j=0; j<numAssigned; j++){

			// calculated distance between frontier[i] and (assigned) frontier[ assigned[j] ]
			int dist = calcDist(frontiers[i].x,frontiers[i].y,frontiers[assigned[j]].x,
					frontiers[assigned[j]].y);
		
			// set utility
			if( dist < 100)
				utility[i] -= ( 1 - (dist/100) ); 

		}//END OF FOR

		// Calculate cost for frontier[i]
		double cost = calcDist( frontiers[i].x,frontiers[i].y, n0->x, n0->y );

		// Calculate Value Function
		double value = (10 * utility[i]) -  cost;

		/* Debug
		 *
		 * printf("\nnodeid : %d, Frontier : %d, Value : %.3f",n0->node_id,i,value);
		 */

		// Find the maximum value function
		if(maxVal < value){
			maxVal = value;
			frontierId = i;
		}


	}// END OF FOR

	// Assign chosen frontier ( based on value function ) to the node
	n0->dstX = frontiers[frontierId].x;
	n0->dstY = frontiers[frontierId].y;

	// update assigned[] and numAssigned
	/*assigned[numAssigned] = frontierId;
	numAssigned++;*/

	return frontierId;

}

/* Activate Cluster */
void activateCluster(struct node* n0,struct node* cluster, int st_loca_size){

	for( int i=0; i< st_loca_size; i++){
		if( cluster[i].pl == 0 )
			if( calcDist( cluster[i].x, cluster[i].y, n0->x,n0->y ) < 55){

				cluster[i].pl = 40;
				gDiscNodes++;

				for( int j=0; j< st_loca_size; j++){
					if( cluster[j].pl == 0)
					if( calcDist( cluster[i].x, cluster[i].y, cluster[j].x, cluster[j].y) < 70){
						cluster[j].pl = 40;
						gDiscNodes++;
					}
				}
			}

	}

	/*
	for(int i=0;i<2;i++)
	if(cluster[i*10].pl == 0)
	if( calcDist( n0->x, n0->y, 100,100 ) < 70 )
		for( int j=0; j<10;j++ )
			cluster[(i*10)+j].pl = 80;

	for(int i=2;i<4;i++)
	if(cluster[(i*10)].pl == 0)
	if( calcDist( n0->x, n0->y, 437,93 ) < 100 )
		for( int j=0; j<10;j++ )
			cluster[(i*10)+j].pl = 80;


	for(int i=4;i<6;i++)
	if(cluster[(i*10)].pl == 0)
	if( calcDist( n0->x, n0->y, 120,380) < 70 )
		for( int j=0; j<10;j++ )
			cluster[(i*10)+j].pl = 80;
			
	for(int i=6;i<8;i++)
	if(cluster[(i*10)].pl == 0)
	if( calcDist( n0->x, n0->y,343,406) < 100 )
		for( int j=0; j<10;j++ )
			cluster[(i*10)+j].pl = 80;
*/
}

void initClusters(struct node *cluster, Point *st_loca, int st_loca_size){

	for( int i=0; i< st_loca_size; i++)
		nodeLocalizedInit(&cluster[i],1,st_loca[i].x,st_loca[i].y,0,0);


}



/*
 * Update the pixel values based on node locations
 */
void updateMap (struct map *);

/*
 * Display the map as an image
 */
void drawMap (struct map *);

/*
 * Clear the map
 */
void cleanMap (struct map *);

/*
 * Destructor
 *
 * 	Destroy the map ( deallocate memory )
 */
void destroyMap (struct map *);
