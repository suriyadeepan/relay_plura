#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "unistd.h"

#include "node.h"
#include "map.h"


double gTime = 0;

// Get necessary parameters from user 
//  Map/Grid Dimensions
double	gMax_X = 500;
double	gMax_Y = 500;
//  #nodes
int	gN = 4;
//	Run Time
double	gRunTime = 100000;
//  Max Speed
double	gMax_Speed = 1;

// set time resolution
double gT_int = 1;

double gTotalDist = 0.0;

// #(discovered static nodes)
int gDiscNodes = 0;

// communication traffic
int traffic = 0;

/* 
 * Frontiers 
 * 	
 * Point	frontiers[200] 
 * 	=> contains coordinates of all frontiers
 *
 * int frontierCount 
 * 	=> num of frontiers
 *
 * int utility[200]
 * 	=> contains utility values of corresponding frontiers
 *
 * int assigned[gN]
 * 	=> contains id of frontiers assigned to nodes
 * 
 * int numAssigned
 * 	=> num of nodes assigned with a frontier
 */
Point frontiers[200];
int frontierCount = 0;
int utility[200];
int assigned[10];
int numAssigned = 0;

Point obst_outline[50];
int obstCount = 0;

double coverage = 0.0;

/*  Clusters **/
// Pointers to clusters 
//struct node *cluster;

// Static node locations
//Point st_loca[40];
//int st_loca_size=0;

/*void CallBackFunc(int event, int x, int y, int flags, void* userdata)
	{
	if  ( event == EVENT_LBUTTONDOWN )
	{
	printf("\n(%d,%d)",x,y);
	st_loca[st_loca_size].x = x;
	st_loca[st_loca_size++].y = y;
	}
	}*/



int main(int argc,char** argv){

	//gN = atoi(argv[1]);
	// #nodes => 1
	gN = 1;

	Mat matrix;

	// Initialize Map
	//initMap(&matrix,gMax_X,gMax_Y);
	matrix = imread("obstacle.png",1);
	//matrix = imread("obstacles.png",1);

	struct node* ni;

	ni = (struct node *)malloc(sizeof(struct node)*gN);

	for(int i=0;i<gN;i++)
		nodeInit(&ni[i],i,1,40);

	loadMap(&matrix, ni, gN);

	int status = -1;

	while(status < 0){

		for(int i=0;i<gN;i++){

			if( abs(calcDist( ni[i].x , ni[i].y, ni[i].dstX, ni[i].dstY)) < 5){

				ni[i].dstX = 450;
				ni[i].dstY = 450;

				// update theta ( direction )
				ni[i].theta = calcTheta( ni[i].x, ni[i].y, ni[i].dstX, ni[i].dstY);

			}

			else{


				// Speed is constant
				double offset = ni[i].x;

				ni[i].x = offset + ( gMax_Speed * 1 ) * cos(ni[i].theta * 22/ (7*180) ) ;

				offset = ni[i].y;
				ni[i].y = offset + ( gMax_Speed * 1 ) * sin(ni[i].theta * 22/ (7*180) ) ;

			}

			if(calcDist( ni[i].x , ni[i].y, 450, 450) < 5)
				return 0;

		}

		loadMap(&matrix, ni, gN);

		if( scan(&matrix, &ni[0]) > 0 ){
			printf("\nObstacle detected...\n");
			Mat roi = matrix( Rect(ni[0].x -40,ni[0].y -40, 80, 80) );
			frontierCount = getFrontiers(&roi, frontiers,utility);

			updateFrontiers(&roi, frontiers, frontierCount);

			printf("\nFrontier Count = %d\n", frontierCount);
			/*if( waitKey(0) == 'q')
				exit(0);
				*/

			Point start(ni[0].x, ni[0].y);
			double x0 = ni[0].x - 40;
			double y0 = ni[0].y - 40;



			while(status < 0){

				Mat roi = matrix( Rect(ni[0].x -40,ni[0].y -40, 80, 80) );
				frontierCount = getFrontiers(&roi, frontiers,utility);

				updateFrontiers(&roi, frontiers, frontierCount);

				//printf("\nFrontier Count = %d\n", frontierCount);

				double x0 = ni[0].x - 40;
				double y0 = ni[0].y - 40;


				int maxId = 0;
				double max = -10000;
				// find best frontier
				for(int k=0; k<frontierCount; k++){

					printf("\n%d : (%d,%d)",k,(int)(frontiers[k].x + x0), (int)(frontiers[k].y + y0) );

					int dist;

					if(obstCount < 2)
						dist = abs(calcDist( start.x , start.y, x0 + frontiers[k].x, y0 + frontiers[k].y)); 

					else
						dist = abs(calcDist( obst_outline[obstCount -2].x , obst_outline[obstCount -2].y, x0 + frontiers[k].x, y0 + frontiers[k].y)); 

					if(dist > max){
						max = dist;
						maxId = k;
					}

				}

				ni[0].dstX = frontiers[maxId].x + x0;
				ni[0].dstY = frontiers[maxId].y + y0;


				obst_outline[obstCount++] = Point(ni[0].dstX, ni[0].dstY); 


				//circle( matrix, Point(ni[0].dstX, ni[0].dstY), 5, Scalar(0,0,255), -1, 8, 0 );
				//printf("\nDst : (%d,%d)",(int)(ni[0].dstX), (int)(ni[0].dstY) );


				loadMap(&matrix, &ni[0],gN);
				//waitKey(0);


				while(calcDist( ni[0].x, ni[0].y, ni[0].dstX, ni[0].dstY) > 5 ){

					// update theta ( direction )
					ni[0].theta = calcTheta( ni[0].x, ni[0].y, ni[0].dstX, ni[0].dstY);

					double offset = ni[0].x;

					ni[0].x = offset + ( gMax_Speed * 0.6 ) * cos(ni[0].theta * 22/ (7*180) ) ;

					offset = ni[0].y;
					ni[0].y = offset + ( gMax_Speed * 0.6 ) * sin(ni[0].theta * 22/ (7*180) ) ;

					loadMap(&matrix, &ni[0],gN);


					if(waitKey(10) == 'q')
						exit(0);
				}


				if( calcDist( ni[0].x, ni[0].y, obst_outline[0].x, obst_outline[0].y ) < 15 && obstCount > 5 ){
					status = 0;
					break;
				}


			}

		}


		if( waitKey(5) == 'q' )
			break;

	}

	// connect all the vertices
	for(int i=0;i<obstCount-1;i++)
		line( matrix, obst_outline[i],obst_outline[i+1], Scalar(0,0,0), 2, 8, 0 );

	int maxX = 0, minX = matrix.cols, maxY=0, minY = matrix.rows;

	for(int i=0;i<obstCount;i++){
		maxX = max(maxX, obst_outline[i].x);
		maxY = max(maxY, obst_outline[i].y);
		minX = min(minX, obst_outline[i].x);
		minY = min(minY, obst_outline[i].y);
	}

	rectangle(matrix, Point(minX, minY), Point(maxX,maxY), Scalar(0,0,255),3,8,0);


	destroyWindow("Update");
	destroyWindow("Frontier Map");

	imshow("traced",matrix);
	waitKey(0);

	matrix.release();


	return 0;

}











