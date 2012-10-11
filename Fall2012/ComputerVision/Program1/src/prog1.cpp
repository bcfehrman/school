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
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "libvision_functions.h"

/**** Defines ****/
#define WINDOW_SIZE_CHOICE CV_WINDOW_NORMAL

/**** Using Statements ****/
using namespace cv;
using namespace std;

/******** Main ***********/

int main( int argc, char *argv[])
{	
	clock_t begin;
	clock_t end;
	Size frame_size;
   Mat orig_image("test.;
   
   
	
	createGausianKernal(FFTKern, standardDeviation, true);
   
	
	cap >> frame_bgr;
   
   frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	

   //Convert to gray scale
   cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);
   
	frame_size = frame_bgr.size();

	//blur.open("edge.avi", CV_FOURCC('X','V','I','D'), 14, frame_size, true);

	namedWindow("Smoothed", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Smoothed", 900, 0);
	namedWindow("Color", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Color", 400, 0);
	

	return 0;
}

