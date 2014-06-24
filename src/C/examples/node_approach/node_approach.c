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
	 * Initialize Nodes
	 */

	// Initialize static node
	nodeLocalizedInit(&ni[0],0,100,50);

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
