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
#include "vision_functions.h"

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
   VideoWriter blur, orig,  orig_edge_highlight;
	int curr_frame = 1;
	clock_t end;
	Size frame_size;
   Size new_frame_size;
	Mat FFTKern(480, 640, CV_32F);
	double time_diff = 0;
	xy_pos window_pos;
	double standardDeviation = 1.5;
	int kernelSize = 5;
	
	if(argc > 1)
		standardDeviation = atof(argv[1]);
	
	if(argc > 2)
		kernelSize = atoi(argv[2]);
	
	cap.open(0); //open the default camera

	//Check to see if the camera opened
	if(!cap.isOpened())
	{
	cout << "Could not open" << endl;
	return -1;
	}
	
	createGausianKernal(FFTKern, 12, true);
	FFTKern.convertTo(FFTKern, CV_32F, 255);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	

	namedWindow("FFTKernel", WINDOW_SIZE_CHOICE);
   imshow("FFTKernel", FFTKern);
  
	Mat frame_bgr;
	cap >> frame_bgr;
   
   //Convert to gray scale
   cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);
   int m = getOptimalDFTSize(frame_bgr.rows);
   int n = getOptimalDFTSize(frame_bgr.cols);
   
	curr_frame++;
	frame_size = frame_bgr.size();
   new_frame_size.width = frame_size.width - (kernelSize / 2) * 2;
   new_frame_size.height = frame_size.height- (kernelSize / 2) * 2;
	blur.open("edge.avi", CV_FOURCC('X','V','I','D'), 14, frame_size, true);
   orig.open("orig.avi", CV_FOURCC('X','V','I','D'), 14, frame_size, true);
   orig_edge_highlight.open("orig_edge_highlight.avi", CV_FOURCC('X','V','I','D'), 14, frame_size, true);

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
      Mat padded;
      Mat frame_orig;
		cap >> frame_orig;
      frame_bgr = frame_orig;
		orig << frame_orig;
		if(frame_bgr.empty())
		break;
//cout << frame_orig.size().width << " " << frame_orig.size().height << endl; 
		
		//Convert the frame t be 32bit floating
		frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	
		
		//Convert to gray scale
		cvtColor(frame_bgr, frame_bgr, CV_BGR2GRAY);
      

  
		
      //copyMakeBorder(frame_bgr, padded, 0, m - frame_bgr.rows, 0, n - frame_bgr.cols, BORDER_CONSTANT, Scalar::all(0));
      
      //    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    //Mat complexI;
    //merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(frame_bgr, frame_bgr);            // this way the result may fit in the source matrix



for(int i = 0; i < FFTKern.size().height; i++)
   for(int j = 0; j < FFTKern.size().width; j++)
   {
      frame_bgr.at<float>(i,j) *= FFTKern.at<float>(i,j);
   }
  // split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
//magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
//Mat magI = planes[0];
//magnitude(frame_bgr, frame_bgr, frame_bgr);

//frame_bgr+= Scalar::all(1);                    // switch to logarithmic scale
//log(frame_bgr, frame_bgr);




 //normalize(magI, magI, 0, 1, CV_MINMAX);
  idft(frame_bgr, frame_bgr);
 //idft(magI, magI);
 normalize(frame_bgr, frame_bgr, 0, 1, CV_MINMAX);
 
 for(int i = 0; i < FFTKern.size().height; i++)
   for(int j = 0; j < FFTKern.size().width; j++)
   {
      if(frame_bgr.at<float>(i,j) < .35)
         frame_bgr.at<float>(i,j) = 0;
      else
         frame_bgr.at<float>(i,j) = 1;
   }
   
    for(int i = 0; i < frame_orig.size().height; i++)
   for(int j = 0; j < frame_orig.size().width; j++)
   {
      if(!frame_bgr.at<float>(i,j))
      {
         for(int k = -1; k < 1; k++)
            for( int l = -1; l<1; l++)
            {
               if(i + k > 0 && i + k < frame_orig.size().height
                  && j + l > 0 && j + l < frame_orig.size().width)
                  {
                     frame_orig.at<Vec3b>(i+k,j+l).val[1] = 190;
                     frame_orig.at<Vec3b>(i+k,j+l).val[0] = 0;
                     frame_orig.at<Vec3b>(i+k,j+l).val[2] = 190;
                  }
            }
      }
   }
   frame_bgr.convertTo(frame_bgr, CV_8U, 255.0);	
		
		//Convert to gray scale
		cvtColor(frame_bgr, frame_bgr, CV_GRAY2BGR);
            orig_edge_highlight << frame_orig;
            blur << frame_bgr;       
            imwrite("me.jpg", frame_bgr);
                   
       		imshow("Smoothed", frame_bgr);
            imshow("Color", frame_orig);

		//Will exit if a window is in focus an a key is pressed
		if(waitKey(30) >= 0) break;
	}

	return 0;
}


