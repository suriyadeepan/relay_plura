#include "node.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"


int nodeInit (struct node* n0, int node_id, int type,int pl){
	
	n0->node_id = node_id;
		
	/*
	 * Generate random values and assign to node positions
	 */
	//srand((int)time(NULL) + node_id);
/*	srand( clock() );
	n0->x = ( (double)rand()/(double)RAND_MAX ) * gMax_X;
	n0->y = ( (double)rand()/(double)RAND_MAX ) * gMax_Y;
	*/
	


	n0->x = gMax_X/2;
	n0->y = gMax_Y/2;
	


	n0->run_time = 0.0;
	n0->rest_time = 0.0;
	n0->speed = 0.0;

	n0->dstX = n0->x;
	n0->dstY = n0->y;

	// set type 
	n0->type = type;

	// set powerlevel
	n0->pl = pl;

	return 0;
}

int nodeLocalizedInit (struct node* n0, int node_id, double x, double y){

	n0->node_id = node_id;
	n0->x = x;
	n0->y = y;

	n0->run_time = 0.0;
	n0->rest_time = 0.0;
	n0->speed = 0.0;

	n0->dstX = x;
	n0->dstY = y;

	return 0;

}

/*
 * Write something about move
 */
int move_rwy (struct node* n0, double dstX, double dstY, double speed, double rest_time){

	n0->dstX = dstX;	n0->dstY = dstY;	n0->speed = speed; n0->rest_time = rest_time;

	// find and set run_time
	n0->run_time = calcDist(n0->x,n0->y,n0->dstX,n0->dstY) / speed;

	// find and set theta
	n0->theta = calcTheta(n0->x,n0->y,n0->dstX,n0->dstY);

	return 0;
}

int move_rwk (struct node* n0, double speed, double theta){
	n0->speed = speed; 	n0->theta = theta; 
	return 0;
}

/*
 * Write something about update
 */
int update_rwy (struct node* n0, double x, double y, double run_time){

	n0->x = x; n0->y = y; n0->run_time = run_time;
	return 0;

}

int update_rwk (struct node* n0){

	double d = n0->speed * gT_int;
	n0->x = n0->x + d*cos( n0->theta * 22 / (7 * 180) );
	n0->y = n0->y + d*sin( n0->theta * 22 / (7 * 180) );
	return 0;

}

/*
 * Write something about trace
 */
void trace (struct node* n0,double g_time){

/*	printf("\n<id: %d,x: %.3f,y: %.3f,dstX: %.3f,dstY: %.3f,theta: %.3f,run_time: %.3f,pause_time : %.3f,speed: %.3f>\n",
			n0->node_id, n0->x,n0->y,n0->dstX,n0->dstY,n0->theta,n0->run_time,n0->rest_time,n0->speed);*/
			
	printf("\n%d %0.3f %.3f %.3f",n0->node_id, g_time, n0->x, n0->y);

}

/*
 * Get a snapshot of parameters of all nodes
 */
void snapshot(struct node* n0,double g_time){

	int i=0;
	for(i=0;i<gN;i++)
		trace(&n0[i],g_time);
}

/*
 * Utility Functions
 *  FIX IT : port these functions to nodeutils.h
 */
double calcTheta(double x1,double y1,double x2,double y2){
	return atan2( y2-y1,x2-x1 )*(180*7)/22; 
}

double calcDist (double x1,double y1,double x2,double y2){
	return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

void calcSines (double x0,double y0,double x1,double y1,double* sv,double* cv){

	double theta = atan2(y1-y0, x1-x0);
	
	*(sv) = sin(theta);
	*(cv) = cos(theta);

}

/*
 * Find the point that is closest to the given point
 * 	from an array of points
 */
int closestPoint(Point *pt, int pointsCount, int x, int y){

	int id = 0;

	int dist = calcDist(pt[id].x,pt[id].y,x,y);

	//printf("\nDist : %d",dist);

	for(int i=0; i<pointsCount; ++i){

		int tempDist = calcDist(pt[i].x, pt[i].y, x, y);

		if( tempDist < dist ){
			dist = tempDist; id = i;
		}

	}

	return id;

}


