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
#define WINDOW_SIZE_CHOICE CV_WINDOW_AUTOSIZE

/**** Using Statements ****/
using namespace cv;
using namespace std;

/****** Prototypes ****/
void updateThreshold(int trackValue, void* userData);
double thresholdVal = 0.0000000005;

/******** Main ***********/

int main( int argc, char *argv[])
{	
   int* defaultThreshold = new int;
	clock_t begin;
	clock_t end;
	Size frame_size;
   Mat origImage1, image1Highlight;
   Mat gaussKernel( 5, 5, CV_32F );
   Mat smoothedImage1;
   Mat xDeriv, GX, yDeriv, GY;
   Mat autoCorrMat1;
   double standardDeviation = 1;
   double sum = 0;
   
   *defaultThreshold = 500;
   
   origImage1 = imread("img/Yosemite/Yosemite1.jpg");
   origImage1.copyTo( image1Highlight);
   
   autoCorrMat1.create( origImage1.size().height, origImage1.size().width, CV_32F);
   
	createGaussianKernal(gaussKernel, standardDeviation);
   
   origImage1.convertTo(smoothedImage1, CV_32F, 1/255.0);	

   //Convert to gray scale
   cvtColor(smoothedImage1, smoothedImage1, CV_BGR2GRAY);
   
   filter2D( smoothedImage1, smoothedImage1, -1, gaussKernel); 

   //hessianMat.create( smoothedImage1.size().height, smoothedImage1.size().width, CV_32F);
   
   createDeriveKernels(GX, GY);
   
   filter2D( smoothedImage1, xDeriv, -1, GX);
   filter2D( smoothedImage1, yDeriv, -1, GY);
   
   createAutoCorrMatrix( smoothedImage1, autoCorrMat1, xDeriv, yDeriv );
   
	namedWindow("Orig", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Orig", 900, 0);
   namedWindow("Smoothed", WINDOW_SIZE_CHOICE);
   createTrackbar("Threshold", "Smoothed", defaultThreshold, 5000,  updateThreshold, NULL); 
	
   for(;;)
   {
      for(int i = 0; i < autoCorrMat1.size().height; i++)
      {
         for(int j = 0; j < autoCorrMat1.size().width; j++ )
         {
            if(autoCorrMat1.at<float>(i,j) > thresholdVal)
            {
               image1Highlight.at<Vec3b>(i,j)[0] = 0;
               image1Highlight.at<Vec3b>(i,j)[1] = 200;
               image1Highlight.at<Vec3b>(i,j)[2] = 0;
            }
            else
            {
               image1Highlight.at<Vec3b>(i,j)[0] = origImage1.at<Vec3b>(i,j)[0];
               image1Highlight.at<Vec3b>(i,j)[1] = origImage1.at<Vec3b>(i,j)[1];
               image1Highlight.at<Vec3b>(i,j)[2] = origImage1.at<Vec3b>(i,j)[2];
            }
            
         }
      }
   
      imshow("Orig", image1Highlight);
      imshow("Smoothed", autoCorrMat1);
      
      if(waitKey(30) >= 0) break;
	}

	return 0;
}

void updateThreshold(int trackValue, void* userData)
{
   thresholdVal = trackValue * pow(10, -12.0);
}

