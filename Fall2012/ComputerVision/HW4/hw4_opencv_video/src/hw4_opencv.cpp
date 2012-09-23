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
	VideoCapture cap;
	int curr_frame = 1;
	clock_t end;
	string filename_in = "vid/lighterFrontyard_160x120_24fps.avi";
	string filename_out_rot = "vid/lighterFontyard_rotated.avi";
	string filename_out_rank = "vid/lighterFrontyard_lowrank.avi";
	int curr_rank = 5;
	int i = 0;
	int num_frames = 0;
	VideoWriter out_rot, out_rank;
	Size frame_size;
	double time_diff = 0;
	xy_pos window_pos;
	
	if(argc > 1)
	  filename_in = argv[1];
	
	cap.open(filename_in); //open the default camera

	//Check to see if the camera opened
	if(!cap.isOpened())
	{
	  cout << "Could not open: " << filename_in << endl;
	  return -1;
	}
	
	Mat frame_bgr;
	cap >> frame_bgr;
	curr_frame++;
	frame_size = frame_bgr.size();

	out_rank.open(filename_out_rank, CV_FOURCC('D','I','V','X'), 14, frame_size, true);
	out_rot.open(filename_out_rot, CV_FOURCC('D','I','V','X'), 14, frame_size, true);

	namedWindow("Corrected", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Corrected", 900, 0);
	
	namedWindow("Low Rank", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Low Rank", 400, 0);
	
	namedWindow("Uncorrected", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Uncorrected", 0, 0);
	
	for(;;)
	{
		/*Uncomment the commented code in this loop 
		  to get FPS timing output. Will add this as 
		  an input option
		*/
		
		//Start clock for FPS timing
		//begin = clock();
		
		//do
		//{
		 //end = clock(); 
		//}while(((double) (end - begin) / CLOCKS_PER_SEC) < 1/30.0);
		
		//Create new bgr matrix and fill it with the
		//current frame from the video camera
		Mat frame_bgr;
		cap >> frame_bgr;
		
		if(frame_bgr.empty())
		  break;
		
		imshow("Uncorrected", frame_bgr);
		
		transpose(frame_bgr, frame_bgr);
		flip(frame_bgr, frame_bgr, 1);		
		transpose(frame_bgr, frame_bgr);

		//Show the corrected image
		imshow("Corrected", frame_bgr);
		
		out_rot << frame_bgr;
		
		//Convert the frame to be 32bit floating
		frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	
		
		//Convert to gray scale
		cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);
		
		//Perform the SVD that gives the U, W, Vt (or U, S, Vt) 
		//except W is just a vector of the singular values
		SVD svd(frame_bgr);	
		
		frame_bgr = svd.u(Range::all(), Range(0,curr_rank)) * 
				Mat::diag(svd.w)(Range(0,curr_rank),Range(0,curr_rank)) *  
				svd.vt(Range(0,curr_rank), Range::all());
		
		imshow("Low Rank", frame_bgr);
		
		frame_bgr.convertTo(frame_bgr, CV_8U, 255.0);
		cvtColor(frame_bgr, frame_bgr, CV_GRAY2BGR);
		
		out_rank << frame_bgr;

		//Timing code
		
		//end = clock();
		//time_diff = (double)(end - begin);
		//cout << 1.0 / (time_diff / CLOCKS_PER_SEC ) << endl;
		//Will exit if a window is in focus an a key is pressed
		if(waitKey(30) >= 0) break;
	}

	return 0;
}
