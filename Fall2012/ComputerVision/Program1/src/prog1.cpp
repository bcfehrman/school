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
#define NUM_SCALES 5

/**** Using Statements ****/
using namespace cv;
using namespace std;

/****** Prototypes ****/
void updateThreshold(int trackValue, void* userData);
double thresholdVal = 0.0000000000005;

/******** Main ***********/

int main( int argc, char *argv[])
{	
   int* defaultThreshold = new int;
	clock_t begin;
	clock_t end;
	Size frame_size;
   Mat origImage1, image1Highlight;
   Mat normLOGKernels[NUM_SCALES];
   Mat gaussGX( 5, 5, CV_32F), gaussGY( 5, 5, CV_32F);
   Mat grayImage1;
   Mat GX, xDeriv, GY, yDeriv;
   Mat autoCorrMat1;
   double standardDeviation = 1.4;
   vector<featVal> featVec;
   
   for(int i = 0; i < NUM_SCALES; i++)
   {
      normLOGKernels[ i ].create( 5, 5, CV_32F );
      createNormLOGKernel( normLOGKernels[ i ], standardDeviation * ( i + 1 ) );
   } 
   
   origImage1 = imread("img/bikes/img1.ppm");
   origImage1.copyTo( image1Highlight);

   //Convert to gray scale
   origImage1.convertTo(grayImage1, CV_32F, 1/255.0);	
   cvtColor(grayImage1, grayImage1, CV_BGR2GRAY);
   
   autoCorrMat1.create( origImage1.rows, origImage1.cols, CV_32F);
   
   createDerivGaussianKernels( gaussGX, gaussGY, standardDeviation);
   
   filter2D( grayImage1, xDeriv, -1, gaussGX);
   filter2D( grayImage1, yDeriv, -1, gaussGY);
   
   createAutoCorrMatrix( grayImage1, autoCorrMat1, xDeriv, yDeriv, thresholdVal );
   suppressNonMaximumsAdaptive( autoCorrMat1, featVec, 30, 250);
   
	namedWindow("Orig", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Orig", 900, 0);
   namedWindow("Smoothed", WINDOW_SIZE_CHOICE);
   //createTrackbar("Threshold", "Smoothed", defaultThreshold, 50000,  updateThreshold, NULL);
    
   for(;;)
   {
      for(int i = 0; i < featVec.size(); i++)
      {
         circle(image1Highlight, featVec.at(i).pos, 5, Scalar(100, 100, 0), 3);
         
         image1Highlight.at<Vec3b>(featVec.at(i).pos.y, featVec.at(i).pos.x)[0] = 0;
         image1Highlight.at<Vec3b>(featVec.at(i).pos.y, featVec.at(i).pos.x)[1] = 200;
         image1Highlight.at<Vec3b>(featVec.at(i).pos.y, featVec.at(i).pos.x)[2] = 0;
      }
   
      imshow("Orig", image1Highlight);
      imshow("Smoothed", autoCorrMat1);
      
      if(waitKey(30) >= 0) break;
	}

	return 0;
}

void updateThreshold(int trackValue, void* userData)
{
   thresholdVal = trackValue * pow(10, -14.0);
}

