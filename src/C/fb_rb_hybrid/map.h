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
 * Find the best possible frontier to explore for a particular node
 *  based on Value Function = ( Utility - Cost )
 *
 * Assign it to the node
 */
int assignBestFrontier(struct node *, Point *, int, int *, int *, int);

/* Find distance bw mobile and static nodes and
 *  activate clusters*/
void activateCluster( struct node *, struct node *, int);

/*
 * Initialize clusters
 */
void initClusters(struct node *, Point *, int);

/*
 * Overlay node locations
 */
void overlayNodes(Mat *, struct node *, int );

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

#endif




