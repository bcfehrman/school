/**
Author: Brian Fehrman
Class: Computer Vision Fall 2012
Professor: Dr. Hoover
Assign: HW3

This program is a twist on Homework 3. It uses OpenCV to capture video feed
from an attached webcam. Each frame is converted to 32bit float, then 
converted to gray scale, then the program performs an SVD on the gray
scale frame. 7 windows are then output. 6 windows show reconstructions of the
gray scale image using varying rank orders. The other window shows the original
gray scale image. The program is setup so that you can easily change the ranks
used, the step size for the ranks, and the overall number of ranks.


**/

/***** Includes *****/
#include <ctime>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

/**** Defines ****/
#define WINDOW_SIZE_CHOICE CV_WINDOW_NORMAL

/**** Using Statements ****/
using namespace cv;
using namespace std;

/**** Structs ******/
struct xy_pos
{
	int x;
	int y;
};

/******** Main ***********/

int main( int argc, char *argv[])
{	
	clock_t begin;
	VideoCapture cap("vid/meFrontyard.webm"); //open the default camera
	ostringstream convert;
	clock_t end;
	int curr_rank = 0;
	int frame_height = 120;
	int frame_width = 160;
	int num_frames = 1;
	Mat frame_lower_rank[num_frames];
	string frame_names[num_frames];
	int i = 0;
	int rank_step = 5;
	string start_name = "Rank ";
	double time_diff = 0;
	xy_pos window_pos;

	//Check to see if the camera opened
	if(!cap.isOpened())
		return -1;

	//Set the capture frame width and height. I set these
	//fairly small on my super weak laptop since it just
	//can't handle the stress =)
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, frame_width);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, frame_height);
	//Create the window for the original gray scale image
	//Move the window so it hopefully won't overlap the other
	//windows
	namedWindow("Corrected", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Corrected", 900, 0);
	
	namedWindow("Uncorrected", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Uncorrected", 0, 0);

	for(;;)
	{
		/*Uncomment the commented code in this loop 
		  to get FPS timing output. Will add this as 
		  an input option
		
		  //Start clock for FPS timing
		  begin = clock();
		*/
		
		//Create new bgr matrix and fill it with the
		//current frame from the video camera
		Mat frame_bgr;
		cap >> frame_bgr;
	
		//Convert the frame to be 32bit floating
		frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	
		
		imshow("Uncorrected", frame_bgr);
		
		transpose(frame_bgr, frame_bgr);
		flip(frame_bgr, frame_bgr, 1);
		
		transpose(frame_bgr, frame_bgr);
		flip(frame_bgr, frame_bgr, 1);
		
		//Convert to gray scale
		//cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);
		
		//cvtColor(frame_bgr, frame_bgr, CV_GRAY2BGR);
		
		//Perform the SVD that gives the U, W, Vt (or U, S, Vt) 
		//except W is just a vector of the singular values
		//SVD svd(frame_bgr);	
		
		//Show the unmodified gray scale image
		imshow("Corrected", frame_bgr);

		/*Timing code
		end = clock();
		timeDiff = (double)(end - begin);
		cout << 1.0 / (timeDiff / CLOCKS_PER_SEC ) << endl;
		*/
		
		//Will exit if a window is in focus an a key is pressed
		if(waitKey(30) >= 0) break;
	}

	return 0;
}