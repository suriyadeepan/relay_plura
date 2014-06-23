/******************************************************************************
*   Name: randwalk.c
*
*   Description: 
*
*   	Implementation of  Random Walk Mobility Model 
*   	Generate mobility trace file for Cooja Simulation
*
*   Author: Suriyadeepan R
*
*   Code Reference :
*   	
*   	Toilers Research Group -- Colorado School of Mines
*
*
******************************************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#undef rand48
#undef DEBUG

#define true  1
#define false 0

#define ZERO 0.00001

double Pi;
double twoPi;
double PiOverTwo;
double PiOverFour;
double threePiOverTwo;

double minX = 0.0;
double minY = 0.0;
double maxX = 0.0;
double maxY = 0.0;

void checkMove(double x, double y, double v, double *newX, double *newY, double *d);

int main(int argc, char *argv[])
{
	int i;
	int numNodes=0;
	int nextNode=0;
	double endTime=0.0, lowest;
	double speedMean=0.0, speedDelta=0.0;
	double travelTime=0.0;
	double *nextEvent;
	double *xLoc, *yLoc;
	double newX, newY, speed, dist, direction;
	double speedLow;
	double speedRange;
	double *timeLeft;
	int *preset;
	double *nextSpeed, *nextDirection;
	char output;

	Pi = 2.0 * asin(1.0);
	twoPi = 2.0 * Pi;
	PiOverTwo = Pi/2.0;
	PiOverFour = Pi/4.0;
	threePiOverTwo = (3.0/2.0) * Pi;

	if (argc == 9)
	{
		numNodes      = atoi(argv[1]);
		maxX          = atof(argv[2]);
		maxY          = atof(argv[3]);
		endTime       = atof(argv[4]);
		speedMean     = atof(argv[5]);
		speedDelta    = atof(argv[6]);
		travelTime    = atof(argv[7]);
		output        = (char)argv[8][0];

		fprintf(stdout, "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		fprintf(stdout, "#\tnumNodes      = %6d\n", numNodes);
		fprintf(stdout, "#\tmaxX          = %9.2f\n", maxX);
		fprintf(stdout, "#\tmaxY          = %9.2f\n", maxY);
		fprintf(stdout, "#\tendTime       = %9.2f\n", endTime);
		fprintf(stdout, "#\tspeedMean     = %9.2f\n", speedMean);
		fprintf(stdout, "#\tspeedDelta    = %9.2f\n", speedDelta);
		fprintf(stdout, "#\ttravelTime    = %9.2f\n", travelTime);
		fprintf(stdout, "#\toutput        = %6c\n", output);
		fprintf(stdout, "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	} else {
		fprintf(stdout, "Usage:  randwalk <number of nodes>\n");
		fprintf(stdout, "                 <max-x> <max-y> <end time>\n");
		fprintf(stdout, "                 <speed mean> <speed delta>\n");
		fprintf(stdout, "                 <travelTime> <'N' or 'G'>\n");
		fprintf(stdout, "                 'N' implies NS2 mobility file\n");
		fprintf(stdout, "                 'G' implies gnuplot path file\n");
		return -1;
	}

	if (output == 'N')
	{
		fprintf(stdout, "# output format is NS2\n");
	} else if (output == 'G') {
		fprintf(stdout, "# output format is gnuplot\n");
		if (numNodes != 1)
		{
			fprintf(stderr, "Gnuplot output is only possible with one mobile node.\n");
			return -1;
		} else {
			fprintf(stdout, "set xrange [%f:%f]\n", minX, maxX);
			fprintf(stdout, "set yrange [%f:%f]\n", minY, maxY);
			fprintf(stdout, "plot \'-\' notitle with linespoints\n");
		}
	} else {
		fprintf(stderr, "Unknown output type requested\n");
		return -1;
	}

  #ifdef rand48
	srand48((int)time(NULL)+(int)getpid());
  #else
	srand((int)time(NULL)+(int)getpid());
  #endif
	speedLow = speedMean-speedDelta;
	speedRange = 2*speedDelta;

	nextEvent = (double*)malloc(sizeof(double)*numNodes);
	xLoc = (double*)malloc(sizeof(double)*numNodes);
	yLoc = (double*)malloc(sizeof(double)*numNodes);
	timeLeft = (double*)malloc(sizeof(double)*numNodes);

	nextSpeed = (double*)malloc(sizeof(double)*numNodes);
	nextDirection = (double*)malloc(sizeof(double)*numNodes);
	preset = (int*)malloc(sizeof(int)*numNodes);

	if (output == 'N')
	{
		fprintf(stdout, "#\tInitial positions:\n");
	}
	for (i=0; i<numNodes; i++)
	{
    #ifdef rand48
		xLoc[i] = drand48()*maxX;
		yLoc[i] = drand48()*maxY;
    #else
		xLoc[i] = ((double)rand()/(double)RAND_MAX)*maxX;
		yLoc[i] = ((double)rand()/(double)RAND_MAX)*maxY;
    #endif
		if (xLoc[i] >= maxX)
		{
			xLoc[i] = maxX - ZERO;
		} else if (xLoc[i] <= minX) {
			xLoc[i] = minX + ZERO;
		}
		if (yLoc[i] >= maxY)
		{
			yLoc[i] = maxY - ZERO;
		} else if (yLoc[i] <= minY) {
			yLoc[i] = minY + ZERO;
		}
		if (output == 'N')
		{
			fprintf(stdout, "$node_(%d) set X_ %.12f\n", i, xLoc[i]);
			fprintf(stdout, "$node_(%d) set Y_ %.12f\n", i, yLoc[i]);
			fprintf(stdout, "$node_(%d) set Z_ %.12f\n", i, 0.0);
		} else if (output == 'G') {
			xLoc[i] = maxX/2.0;
			yLoc[i] = maxY/2.0;
			fprintf(stdout, "%.12f %.12f\n", xLoc[i], yLoc[i]);
		}

		nextEvent[i] = 0.0;

		preset[i]=0;
		nextDirection[i]=0.0;
		nextSpeed[i]=0.0;
	}

	if (output == 'N')
	{
		fprintf(stdout, "\n\n#\tMovements:\n");
	}

	lowest = endTime;  /* initialize high so all starting movements are
	                      scheduled and output */
  #ifdef rand48
	speed = drand48()*speedRange + speedLow;
	direction = drand48()*twoPi;
  #else
	speed = ((double)rand()/(double)RAND_MAX)*speedRange + speedLow;
	direction = ((double)rand()/(double)RAND_MAX)*twoPi;
  #endif
	for (i=0; i<numNodes; i++)
	{
		timeLeft[i] = travelTime;
	}
	while (lowest <= endTime)
	{
		if(preset[nextNode])
		{
			speed=nextSpeed[nextNode];
			direction=nextDirection[nextNode];
		}

		newX = xLoc[nextNode] + speed * cos(direction) * timeLeft[nextNode];
		newY = yLoc[nextNode] + speed * sin(direction) * timeLeft[nextNode];
		//fprintf(stdout, "# timeLeft[%d]=%.20f\n", nextNode, timeLeft[nextNode]);

#ifdef DEBUG
		fprintf(stdout, "# Calling checkMove %.12f %.12f, direction=%.10f\n", newX, newY, direction);
#endif
		checkMove(xLoc[nextNode], yLoc[nextNode], speed, &newX, &newY, &direction);

		if (newX >= maxX)
		{
			newX = maxX - ZERO;
		} else if (newX <= minX) {
			newX = minX + ZERO;
		}
		if (newY >= maxY)
		{
			newY = maxY - ZERO;
		} else if (newY <= minY) {
			newY = minY + ZERO;
		}

		if (output == 'N')
		{
			fprintf(stdout, "$ns_ at %.10f \"$node_(%d) setdest %.10f %.10f %.10f\"\n",
			        nextEvent[nextNode], nextNode, newX, newY, speed);
		} else if (output == 'G') {
			fprintf(stdout, "%.12f %.12f # node %d at %.10f speed=%.10f\n", newX, newY, nextNode, nextEvent[nextNode], speed);
		}
		dist = sqrt((newX-xLoc[nextNode])*(newX-xLoc[nextNode]) + (newY-yLoc[nextNode])*(newY-yLoc[nextNode]));
		xLoc[nextNode] = newX;
		yLoc[nextNode] = newY;
		timeLeft[nextNode] -= dist/speed;
		//fprintf(stdout, "# timeLeft[%d]=%.20f\n\n", nextNode, timeLeft[nextNode]);
		nextEvent[nextNode] += dist/speed;

		if (timeLeft[nextNode] <= ZERO)
		{
			preset[nextNode]=0;
			timeLeft[nextNode] = travelTime;
      #ifdef rand48
			speed = drand48()*speedRange + speedLow;
			direction = drand48()*twoPi;
      #else
			speed = ((double)rand()/(double)RAND_MAX)*speedRange + speedLow;
			direction = ((double)rand()/(double)RAND_MAX)*twoPi;
      #endif
		}
		else
		{
			preset[nextNode]=1;
			nextSpeed[nextNode]=speed;
			nextDirection[nextNode]=direction;
		}

		/* find new lowest */
		lowest = endTime + 1.0;
		for (i=0; i<numNodes; i++)
		{
			if (nextEvent[i] < lowest)
			{
				lowest = nextEvent[i];
				nextNode = i;
			} /* if (nextEvent[i] <= lowest) */
		} /* for (i=0; i<numNodes; i++) */
	} /* while (lowest <= endTime) */

	if (output == 'N')
	{
		fprintf(stdout, "\n\n\n");
	} else if (output == 'G') {
		fprintf(stdout, "e\n\n");
	}

	free(nextEvent);
	free(xLoc);
	free(yLoc);
	free(timeLeft);

	return 0;
}

void checkMove(double x, double y, double v, double *newX, double *newY, double *d)
{
	double xTime, yTime;
	double surfaceAngle;

	/*************************************/
	/*                                   */
	/*           |           |           */
	/*     3     |     2     |     1     */
	/*           |           |           */
	/* --------------------------------- */
	/*           |           |           */
	/*           |           |           */
	/*     4     |           |     0     */
	/*           |           |           */
	/*           |           |           */
	/* --------------------------------- */
	/*           |           |           */
	/*     5     |     6     |     7     */
	/*           |           |           */
	/*                                   */
	/*************************************/

#ifdef DEBUG
	fprintf(stdout, "# In checkMove %.12f %.12f, direction=%.10f\n", *newX, *newY, *d);
#endif
	if ((*newX > maxX) && (*newY < maxY) && (*newY > minY))          /*  case 0  */
	{
		//fprintf(stdout, "0\n");
		xTime = fabs(maxX-x)/fabs(v*cos(*d));
		surfaceAngle = PiOverTwo;
		*newX = maxX;
		*newY = y + v*sin(*d)*xTime;
		*d = 2*surfaceAngle - (*d);
	} else if ((*newX > maxX) && (*newY > maxY)) {                  /*  case 1  */
		//fprintf(stdout, "1\n");
		xTime = fabs(maxX-x)/fabs(v*cos(*d));
		yTime = fabs(maxY-y)/fabs(v*sin(*d));
		if (xTime < yTime)
		{ /* hit the side first */
			surfaceAngle = PiOverTwo;
			*newX = maxX;
			*newY = y + v*sin(*d)*xTime;
			*d = 2*surfaceAngle - (*d);
		} else if (yTime < xTime) { /* hit top first */
			surfaceAngle = Pi;
			*newX = x + v*cos(*d)*yTime;
			*newY = maxY;
			*d = 2*surfaceAngle - (*d);
		} else { /* hits corner */
			*newX = maxX;
			*newY = maxY;
			*d = Pi + (PiOverFour);
		}
	} else if ((*newY > maxY) && (*newX < maxX) && (*newX > minX)) {   /*  case 2  */
		//fprintf(stdout, "2\n");
		yTime = fabs(maxY-y)/fabs(v*sin(*d));
		surfaceAngle = Pi;
		*newX = x + v*cos(*d)*yTime;
		*newY = maxY;
		*d = 2*surfaceAngle - (*d);
	} else if ((*newX < minX) && (*newY > maxY)) {                  /*  case 3  */
		//fprintf(stdout, "3\n");
		xTime = fabs(x-minX)/fabs(v*cos(*d));
		yTime = fabs(maxY-y)/fabs(v*sin(*d));
		if (xTime < yTime)
		{ /* hit the side first */
			surfaceAngle = PiOverTwo;
			*newX = minX;
			*newY = y + v*sin(*d)*xTime;
			*d = 2*surfaceAngle - (*d);
		} else if (yTime < xTime) { /* hit top first */
			surfaceAngle = Pi;
			*newX = x + v*cos(*d)*yTime;
			*newY = maxY;
			*d = 2*surfaceAngle - (*d);
		} else { /* hits corner */
			*newX = minX;
			*newY = maxY;
			*d = twoPi - (PiOverFour);
		}
	} else if ((*newX < minX) && (*newY < maxY) && (*newY > minY)) { /*  case 4  */
		//fprintf(stdout, "4\n");
		xTime = fabs(minX-x)/fabs(v*cos(*d));
		surfaceAngle = PiOverTwo;
		*newX = minX;
		*newY = y + v*sin(*d)*xTime;
		*d = 2*surfaceAngle - (*d);
	} else if ((*newX < minX) && (*newY < minY)) {                  /*  case 5  */
		//fprintf(stdout, "5\n");
		xTime = fabs(x-minX)/fabs(v*cos(*d));
		yTime = fabs(y-minY)/fabs(v*sin(*d));
		if (xTime < yTime)
		{ /* hit the side first */
			surfaceAngle = PiOverTwo;
			*newX = minX;
			*newY = y + v*sin(*d)*xTime;
			*d = 2*surfaceAngle - (*d);
		} else if (yTime < xTime) { /* hit top first */
			surfaceAngle = Pi;
			*newX = x + v*cos(*d)*yTime;
			*newY = minY;
			*d = 2*surfaceAngle - (*d);
		} else { /* hits corner */
			*newX = minX;
			*newY = minY;
			*d = PiOverFour;
		}
	} else if ((*newY < minY) && (*newX < maxX) && (*newX > minX)) {   /*  case 6  */
		//fprintf(stdout, "6\n");
		yTime = fabs(y-minY)/fabs(v*sin(*d));
		surfaceAngle = Pi;
		*newX = x + v*cos(*d)*yTime;
		*newY = minY;
		*d = 2*surfaceAngle - (*d);
	} else if ((*newX > maxX) && (*newY < minY)) {                  /*  case 7  */
		//fprintf(stdout, "7\n");
		xTime = fabs(maxX-x)/fabs(v*cos(*d));
		yTime = fabs(y-minY)/fabs(v*sin(*d));
		if (xTime < yTime)
		{ /* hit the side first */
			surfaceAngle = PiOverTwo;
			*newX = maxX;
			*newY = y + v*sin(*d)*xTime;
			*d = 2*surfaceAngle - (*d);
		} else if (yTime < xTime) { /* hit top first */
			surfaceAngle = Pi;
			*newX = x + v*cos(*d)*yTime;
			*newY = minY;
			*d = 2*surfaceAngle - (*d);
		} else { /* hits corner */
			*newX = maxX;
			*newY = minY;
			*d = Pi - (PiOverFour);
		}
	}
#ifdef DEBUG
	fprintf(stdout, "# After checkMove %.12f %.12f, direction=%.10f\n", *newX, *newY, *d);
#endif

	return;
}
