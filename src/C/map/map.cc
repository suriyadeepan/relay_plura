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

void initPlot(Mat* plotImg){

	*plotImg = Mat(500, 1500, CV_8UC3);
	plotImg->setTo(Scalar(255,255,255));

}

double plot(Mat* matrix,Mat* plotImg,int gTime,int plotStatus){


	/* Calculate Coverage ( ratio of white to black )
	 *
	 * 	1] convert *matrix to GrayScale
	 * 	2] Binary threshold GrayScale image
	 * 	3] call countNonZero() method
	 */
	char str[10];

	Mat dst;
	Mat cannied;
	vector<vector<Point> > contours;

	cvtColor(*matrix,dst,CV_BGR2GRAY,0);
	threshold(dst,dst,10,255,THRESH_BINARY);


	Canny(dst, cannied , 100, 200, 3, false);

	findContours( cannied, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	// CV_RETR_LIST
	
	Mat contourImg;
	contourImg = Mat(500, 500, CV_8UC3);

	//printf("\nNo. of contour : %d", (int)contours.size() );

	for (size_t idx = 0; idx < contours.size(); idx++) {
		drawContours(contourImg, contours, idx, Scalar(255,0,0) );
	}

	printf("\n--------------------------------------------\n");
	for(size_t i=0; i<contours.size(); i++ ){
		   // use contours[i] for the current contour
			 for(size_t j=0; j<contours[i].size(); j++ ){
			           // use contours[i][j] for current point
								 if(j%10 == 0)
									 printf("\n%d %d",contours[i][j].x,contours[i][j].y);
			 }
	}

	printf("\n--------------------------------------------\n");



	double coverage = (double)( countNonZero(dst)/(gMax_X*gMax_Y) );

	circle( *plotImg, Point(gTime/5, 500 - (coverage*500) ), 2, Scalar(0,255,0), -1, 8, 0 );

	if(plotStatus != 0)
		imshow("My Plot",*plotImg);

	//imshow("Edge Detection",cannied);
	imshow("Mat00",contourImg);

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
