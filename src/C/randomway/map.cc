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

/*
 * Call mapInitAll() along with loadMap()
 */

//int mapInitAll (struct map *,int, int, struct node *, int);


/*
 * Load node locations into map
 */
int loadMap(Mat* matrix,struct node *n0, int nodeCount){

	int x,y;
	//char str[10];
	/*
	 * Iterate through the node locations till nodeCount
	 * 	Draw circles at those locations in the map
	 */
	for(int i=0; i<nodeCount; i++){

		x = (int) *(&n0[i].x);
		y = (int) *(&n0[i].y);


		circle( *matrix, Point(x,y), 40, Scalar(255,255,255), -1, 8, 0 );
		circle( *matrix, Point(x,y), 10, Scalar(0,255,0), -1, 8, 0 );

		//sprintf(str,"%d: (%d,%d)",*(&n0[i].node_id),x,y);
		//putText(*matrix, str,Point(30,30),FONT_HERSHEY_PLAIN,1, Scalar(0,0,255),1,8,false);

	}// END OF FOR

	
	return 0;

}


// Calc Coverage
double getCoverage(Mat* matrix ){


	/* Calculate Coverage ( ratio of white to black )
	 *
	 * 	1] convert *matrix to GrayScale
	 * 	2] Binary threshold GrayScale image
	 * 	3] call countNonZero() method
	 */
	Mat dst;
	cvtColor(*matrix,dst,CV_BGR2GRAY,0);
	threshold(dst,dst,10,255,THRESH_BINARY);

	double coverage = (double)( countNonZero(dst)/(gMax_X*gMax_Y) );

	// deallocate memory
	dst.release();

	return coverage;

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
