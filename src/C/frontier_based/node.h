#ifndef NODE_H
#define NODE_H

#include "opencv2/opencv.hpp"

using namespace cv;

struct node{
	
	// set in the constructor
	int node_id;

	// current location of node
	double x,y;

	// destination location updated everytime
	//  node stops ( when move() function is called )
	double dstX,dstY;

	// updated everytime node stops ( when move() function is called )
	double speed;

	/* -- Direction --
	 *
	 * 		theta = arctan( (dstX - x) / (dstY - y) )
	 *
	 */
	double theta;

	/* Updated every TIME_RESOLUTION time units
	0 * 	
	 * 	run_time = run_time - TIME_RESOLUTION
	 *  
	 * When run_time becomes 0 move() function is called and
	 *  new destination and speed are set
	 */
	double run_time;

	/* its the Pause time
	 *  selected randomly everytime the node stops
	 */
	double rest_time;

	/*
	 * Type -> mobile/static
	 */
	int type;

	/*
	 * Set Power level
	 */
	int pl;
	

};

// Constructors

int nodeInit (struct node* ,int, int,int);
int nodeLocalizedInit (struct node*,int,double,double);

// General Utility Functions

/*
 * Write something about move
 */
int move_rwy (struct node*,double,double,double,double);
int move_rwk (struct node*,double,double);


/*
 * Write something about update
 */
int update_rwy (struct node*,double,double,double);
int update_rwk (struct node*);


/*
 * Write something about trace
 */
void trace (struct node*,double);

/*
 * Get a snapshot of parameters of all nodes
 */
void snapshot (struct node*,double);

/*
 * Utility Functions
 *  FIX IT : port these functions to nodeutils.h
 */
double calcTheta(double ,double ,double ,double );

double calcDist (double ,double ,double ,double );

int closestPoint(Point *, int, int, int);


/*
 * extern Global Variables
 */
extern double gMax_X;
extern double gMax_Y;
extern double gMax_Speed;
extern double gMax_Pause;
extern int gN;
extern double gRunTime;

extern double gT_int;

#endif
