#ifndef MAP_H
#define MAP_H

// Include opencv2 header
#include "opencv2/opencv.hpp"
#include "node.h"

using namespace cv;


/*
 * Methods in map.h
 */

/*
 * Constructors:
 *
 * 	1] Initialize Map with sizeX and size Y
 *
 */

int initMap (Mat *, int, int );

/*
 * Load node locations into map
 */
int loadMap(Mat *, struct node *, int);

/*
 * Calculate Histogram
 */
double getCoverage(Mat *);

/*
 * Find the frontiers using Edge Detection Filter
 */
int getFrontiers(Mat *,Point *,int *);

/*
 * Update the map with frontiers
 */
void updateFrontiers(Mat *, Point *, int);

/*
 * calculate utitliy
 */
int calcUtil(double, int, double, double, int, int, int);

/*
 * Find the destination  with highest utility
 */
void setBestDestination(struct node *,Mat *mat);

/*
 * Find the best possible frontier to explore for a particular node
 *  based on Value Function = ( Utility - Cost )
 *
 * Assign it to the node
 */
int assignBestFrontier(struct node *, Point *, int, int *, int *, int);


/*
 * Update the pixel values based on node locations
 */
void updateMap (Mat *);

/*
 * Display the map as an image
 */
void drawMap (Mat *);

/*
 * Clear the map
 */
void cleanMap (Mat *);

/*
 * Destructor
 *
 * 	Destroy the map ( deallocate memory )
 */
void destroyMap (Mat *);

/* Global matrix */
extern Mat bwMat;
extern double gC;

#endif




