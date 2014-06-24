#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

#include "node.h"

double gTime = 0;

int main(){

	// Get necessary parameters from user 
	//  Map/Grid Dimensions
	gMax_X = 250;
	gMax_Y = 250;
	//  #nodes
	gN = 2;
	//	Run Time
	gRunTime = 1000;
	//  Max Speed
	gMax_Speed = 3;

	// set time resolution
	gT_int = 0.2;

	double step = 0.2;

	int i;

	// Pointer to nodes
	struct node* ni;

	ni =(struct node *)malloc(sizeof(struct node)*gN);
	

	/*
	 * Initialize multiple clusters of static nodes
	 *  a bunch of nodes within the range of each other 
	 *   form a cluster  
	 *  range => 50 ( distance units )
	 */

	/*
	 * Clusters are created manually here
	 *  FIX IT : need a generalized technique to automate this
	 */

	// cluster 1
	nodeLocalizedInit(&ni[0],0,79,20);
	nodeLocalizedInit(&ni[1],0,60,33);
	nodeLocalizedInit(&ni[2],0,66,20);
	nodeLocalizedInit(&ni[3],0,84,15);

	// cluster 2
	nodeLocalizedInit(&ni[4],0,180,150);
	nodeLocalizedInit(&ni[5],0,190,166);
	nodeLocalizedInit(&ni[6],0,174,144);

	// cluster 3
	nodeLocalizedInit(&ni[7],0,240,220);
	nodeLocalizedInit(&ni[8],0,231,230);
	nodeLocalizedInit(&ni[9],0,250,235);


	// Initialize mobile node
	nodeLocalizedInit(&ni[1],1,200,50);


	// Start Running till end of gRunTime
	while(gTime <=  gRunTime){


		if(ni[1].x > 230)
			step = +0.2;

		if(ni[1].x < 10)
			step = -0.2;


		int offset = ni[1].x;
		ni[1].x = offset - step;

 	 //Snapshot of node locations @ time "gTime"
	 snapshot(&ni[0],gTime);

	 gTime += gT_int;

	}// end of WHILE

	return 0;

}
