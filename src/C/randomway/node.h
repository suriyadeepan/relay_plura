#ifndef NODE_H
#define NODE_H

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
	 * 	
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
	

};

// Constructors

int nodeInit (struct node* ,int);
int nodeLocalizedInit (struct node*,int,double,double);

// General Utility Functions

/*
 * Write something about move
 */
int move (struct node*,double,double,double,double);


/*
 * Write something about update
 */
int update (struct node*,double,double,double);


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


/*
 * Global Variables
 */
double gMax_X;
double gMax_Y;
double gMax_Speed;
double gMax_Pause;
double gN;
double gRunTime;

#endif
