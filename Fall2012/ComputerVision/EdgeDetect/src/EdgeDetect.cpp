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

/**** Structs ******/
struct xy_pos
{
	int x;
	int y;
};

void updateSTDEdgeValue(int trackValue, void* userData);
void updateCutoffEdgeValue(int trackValue, void* userData);
double standardDeviation = 1.5;
double cuttOff = .4;
Mat FFTKern(480, 640, CV_32F);

/******** Main ***********/

int main( int argc, char *argv[])
{	
	clock_t begin;
	VideoCapture cap;
   VideoWriter blur;
	clock_t end;
	Size frame_size;
	//double time_diff = 0;
	
	cap.open(0); //open the default camera

	//Check to see if the camera opened
	if(!cap.isOpened())
	{
	cout << "Could not open" << endl;
	return -1;
	}
	
	createGausianKernal(FFTKern, standardDeviation, true);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
   int* fftInit = new int;
   *fftInit = 300;

	namedWindow("FFTKernel", WINDOW_SIZE_CHOICE);
   createTrackbar("FFTKernelSTD", "FFTKernel", fftInit, 500,  updateSTDEdgeValue, NULL); 
   createTrackbar("FFTKernelCutoff", "FFTKernel", 0, 500, updateCutoffEdgeValue, NULL); 
   createTrackbar("FFTKernelCutoff3", "FFTKernel", 0, 20, updateCutoffEdgeValue, NULL); 
   imshow("FFTKernel", FFTKern);
  
	Mat frame_bgr;
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
	
	
	for(;;)
	{
		/*Uncomment the commented code in this loop 
		to get FPS timing output. Will add this as 
		an input option
		*/
		
		//Start clock for FPS timing
		begin = clock();
		
		//do
		//{
		//end = clock(); 
		//}while(((double) (end - begin) / CLOCKS_PER_SEC) < 1/30.0);
		
		//Create new bgr matrix and fill it with the
		//current frame from the video camera
		Mat frame_bgr;
      Mat frame_orig;
		cap >> frame_orig;
      frame_bgr = frame_orig;
      
		if(frame_bgr.empty())
         break;
		
		//Convert the frame t be 32bit floating
		frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	
		
		//Convert to gray scale
		cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);

      fftEdgeDetect( frame_bgr, frame_orig, FFTKern, cuttOff);
      
      frame_bgr.convertTo(frame_bgr, CV_8U, 255.0);	
         
      //Convert to gray scale
      cvtColor(frame_bgr, frame_bgr, CV_GRAY2BGR);     
      
      end = clock();
      
      cout << double (end - begin) / CLOCKS_PER_SEC << endl;
             
      imshow("Smoothed", frame_bgr);
      imshow("Color", frame_orig);
      FFTKern.convertTo(FFTKern, CV_32F, 255);
      imshow("FFTKernel", FFTKern);
      FFTKern.convertTo(FFTKern, CV_32F, 1/255.0);

		//Will exit if a window is in focus an a key is pressed
		if(waitKey(30) >= 0) break;
	}

	return 0;
}

 void updateSTDEdgeValue(int trackValue, void* userData)
 {
    standardDeviation = trackValue / 20.0;
    createGausianKernal(FFTKern, standardDeviation, true);
 }
 void updateCutoffEdgeValue(int trackValue, void* userData)
 {
    cuttOff = trackValue / 1000.0;
 }


