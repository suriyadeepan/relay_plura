#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "opencv2/opencv.hpp"

using namespace cv;


//  Map/Grid Dimensions
double	gMax_X = 500;
double	gMax_Y = 500;

Mat im;

double calcDistance(int x0, int y0, int x1, int y1){

	// 5] Calculate distance b/w src and dst
	double D = sqrt( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) );

	return D;

}

void calcSine(int x0, int y0, int x1, int y1, double *sv, double *cv){

	// 3] find theta
	double theta = atan2( y1-y0, x1-x0);
	//	printf("\n\nTheta : %.3f\n\n",theta*(180*7)/22);

	// 4] find sine and cosine
	*(sv) = sin(theta);
	*(cv) = cos(theta);

	//	printf("\n\nSine : %.3f %.4f\n",*sv,*cv);

}

int calcUtil(double D, int d, double sv, double cv, int x0, int y0){

	int util = 0;

	// calculate utility
	for(int i=0;i<(int)(D/d);i++){

		// print points along the line
		int x = (int) ( d*(i+1)*cv )  + x0;
		int y = (int) ( d*(i+1)*sv )  + y0;


		if( (int)(im.at<unsigned char>(x,y)/255)  == 0 )
			util += 1;

	}

	return util;

}


int main(int argc,char** argv){

	// 1] Read dummy Image
	Mat src = imread(argv[1],1);

	cvtColor(src,im,CV_BGR2GRAY);

	imshow("src",im);

	Mat dst = Mat(gMax_X/20,gMax_Y/20,CV_8U);

	// 2] Choose 2 points
	Point pSrc = Point(25,25);
	Point pDst = Point(400,400);

	int x0 = 25;
	int y0 = 25;

	int x1 = 400;
	int y1 = 400;

	int x2,y2;


	// set step
	int d = 15;
	double D = 0.0;


	//	int utilCount = 0;
	int maxUtil = -100000;

	double sv=0.0,cv=0.0;

	for(x1=0; x1 <= gMax_X; x1+=25){

		for(y1=0; y1 <= gMax_Y; y1+=25){

			// get dist
			D = calcDistance(x0,y0,x1,y1 );

			// get sine, cosine values
			calcSine(x0,y0,x1,y1,&sv,&cv);

			// find util
			int util = calcUtil(D,d,sv,cv,x0,y0);

			if(maxUtil < util){
				maxUtil = util;
				x2 = x1;
				y2 = y1;
			}

			printf("\n\nUtility (%d,%d) --> (%d,%d)  : %d\n\n",x0,y0,x1,y1,util);

			waitKey(0);
		}

	}

	printf("\n\nBest damn Utility : (%d,%d) --> (%d,%d) : %d\n\n",x0,y0,x2,y2,maxUtil);


	waitKey(0);

	return 0;

}
