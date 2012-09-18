//is HW3 with the SVD but will try to use video capture

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

#define WINDOW_SIZE_CHOICE CV_WINDOW_NORMAL

using namespace cv;
using namespace std;

struct xyPos
{
	int x;
	int y;
};

int main( int argc, char *argv[])
{	
	int numFrames = 6;
	int rankStep = 5;
	string frameNames[numFrames];
        VideoCapture cap(1); //open the default camera
	Mat frame_lower_rank[numFrames];
	ostringstream convert;
	int currRank = rankStep;
	int i = 0;
	int frameWidth = 160;
	int frameHeight = 120;
	xyPos windowPositions[numFrames];
	string startName = "Rank ";
	clock_t begin;
	clock_t end;
	double timeDiff = 0;

	if(!cap.isOpened()) //check if we succeeded
		return -1;

	for(i = 0; i < numFrames; i++)
	{
		convert << i * rankStep + rankStep;
		frameNames[i] = startName + convert.str();
		namedWindow(frameNames[i], WINDOW_SIZE_CHOICE);
		convert.clear();
		convert.str("");		

		windowPositions[i].x = (i % 2) * frameWidth * 2.5 + 100;
		windowPositions[i].y = i / 2 * frameHeight * 2.5;
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);
	namedWindow("Non SVD", WINDOW_SIZE_CHOICE);

	for(;;)
	{
		begin = clock();
		
		Mat frame_bgr;
		
		cap >> frame_bgr;
	
		frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	
		
		//Convert to gray scale
		cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);
		imshow("Non SVD", frame_bgr);
		cvMoveWindow("Non SVD", 900, 0);

		//Perform the SVD that gives the U, W, Vt (or U, S, Vt) 
		//except W is just a vector of the singular values
		SVD svd(frame_bgr);
		
		currRank = rankStep;
		
		for(i = 0; i < numFrames; i++)
		{
			//Reconstruct the iamge, the W vector is converted into a diagonal 
			//matrix so that it plays nicely, might be able to make that better
			frame_lower_rank[i] = svd.u(Range::all(), Range(0,currRank)) * 
				Mat::diag(svd.w)(Range(0,currRank),Range(0,currRank)) *  
				svd.vt(Range(0,currRank), Range::all());

			currRank += rankStep;
		}

		for(i = 0; i < numFrames; i++)
		{
			imshow(frameNames[i], frame_lower_rank[i]);
			cvMoveWindow( frameNames[i].c_str(), windowPositions[i].x, windowPositions[i].y); 
		}			

		end = clock();
		timeDiff = (double)(end - begin);
		cout << 1.0 / (timeDiff / CLOCKS_PER_SEC ) << endl;
		if(waitKey(30) >= 0) break;
	}

	return 0;
}
