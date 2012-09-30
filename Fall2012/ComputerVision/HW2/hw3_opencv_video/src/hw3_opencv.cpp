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
    VideoCapture cap(0); //open the default camera
	ostringstream convert;
	clock_t end;
	int curr_rank = 0;
	int frame_height = 240;
	int frame_width = 320;
	int num_frames = 6;
	Mat frame_lower_rank[num_frames];
	string frame_names[num_frames];
	int i = 0;
	int rank_step = 5;
	string start_name = "Rank ";
	double time_diff = 0;
	xy_pos window_pos;

	//Check to see if the camera opened
	if(!cap.isOpened())
	{
	  cout << "Video error" <<endl;
		return -1;
	}
	//Create the windows for each lower rank frame that will
	//be displayed and set the position for each window.
	//I have been playing around with the window positions.
	//I think that the problem might be that the frame height
	//and width aren't the actual size that the window is made
	//to be. I don't know though, good enough for now
	for(i = 0; i < num_frames; i++)
	{
		//Convert the rank number of the frame
		//from an integer type to a string type
		convert << i * rank_step + rank_step;
		frame_names[i] = start_name + convert.str();
		
		//Create the window for that rank number and set it's
		//sizing option to the WINDOW_SIZE_CHOICE macro that
		//we defined
		namedWindow(frame_names[i], WINDOW_SIZE_CHOICE);
		
		//Clear the conversion buffer otherwise it will
		//will keep the old values in the buffer and
		//append the new values to the old ones
		convert.clear();
		convert.str("");		

		//Set the window positions roughly based on frame height
		//and width with a few "fudge factors" in there for now
		window_pos.x = (i % 2) * frame_width * 2.5 + 100;
		window_pos.y = i / 2 * frame_height * 2.5;
		cvMoveWindow(frame_names[i].c_str(), window_pos.x, window_pos.y);
	}

	//Set the capture frame width and height. I set these
	//fairly small on my super weak laptop since it just
	//can't handle the stress =)
	cap.set(CV_CAP_PROP_FRAME_WIDTH, frame_width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, frame_height);
	cap.get(CV_CAP_PROP_FPS);
	//Create the window for the original gray scale image
	//Move the window so it hopefully won't overlap the other
	//windows
	namedWindow("Non SVD", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Non SVD", 900, 0);

	for(;;)
	{
		/*Uncomment the commented code in this loop 
		  to get FPS timing output. Will add this as 
		  an input option*/
		
		//Start clock for FPS timing
		begin = clock();
		
		
		//Create new bgr matrix and fill it with the
		//current frame from the video camera
		Mat frame_bgr;
		cap >> frame_bgr;
	
		//Convert the frame to be 32bit floating
		frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	
		
		//Convert to gray scale
		cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);
		
		//Perform the SVD that gives the U, W, Vt (or U, S, Vt) 
		//except W is just a vector of the singular values
  		SVD svd(frame_bgr);
		
		//Reset the currRank iteration variable to start at
		//the desired rank step
		curr_rank = rank_step;
		
		//Reconstruct each image using a lower rank representation
		//The W vector is converted into a diagonal matrix so
		//that it plays nicely, might be able to make that better
		for(i = 0; i < num_frames; i++)
		{
			frame_lower_rank[i] = svd.u(Range::all(), Range(0,curr_rank)) * 
				Mat::diag(svd.w)(Range(0,curr_rank),Range(0,curr_rank)) *  
				svd.vt(Range(0,curr_rank), Range::all());

			curr_rank += rank_step;
		}

		//Show each lower rank image in its corresponding window that we created earlier.
		for(i = 0; i < num_frames; i++)
		{
			imshow(frame_names[i], frame_lower_rank[i]);
		}		
		
		//Show the unmodified gray scale image
		imshow("Non SVD", frame_bgr);

		/*Timing code*/
		end = clock();
		time_diff = (double)(end - begin);
		cout << 1.0 / (time_diff / CLOCKS_PER_SEC ) << endl;

		
		
		//Will exit if a window is in focus an a key is pressed
		if(waitKey(30) >= 0) break;
	}

	return 0;
}
