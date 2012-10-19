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
float thresholdVal = .001;

/******** Main ***********/

int main( int argc, char *argv[])
{	
   int* defaultThreshold = new int;
	clock_t begin;
	clock_t end;
	Size frame_size;
   Mat origImage1, image1Highlight;
   Mat origImage2, image2Highlight;
   Mat normLOGKernels[NUM_SCALES];
   Mat gaussGXKernels[NUM_SCALES];
   Mat gaussGYKernels[NUM_SCALES];
   Mat grayImage1, grayImage2;
   Mat GX, xDeriv, GY, yDeriv, xyDeriv;
   Mat autoCorrMat1, autoCorrMat2;
   const float derivStandardDeviation = 1.5;
   const float smoothStandardDeviation = 2;
   vector<featVal> featVec1, featVec2;
   const int numKeep = 100;
   const int radiusSuppress = 25;
   Mat smoothGauss(5, 5, CV_32F);
   VideoCapture cap(0);
   
   createGaussianKernel( smoothGauss, smoothStandardDeviation );
  
   for(int i = 0; i < NUM_SCALES; i++)
   {
      normLOGKernels[ i ].create( 5, 5, CV_32F );
      createNormLOGKernel( normLOGKernels[ i ],  smoothStandardDeviation * ( i + 1 ) );
      
      gaussGXKernels[ i ].create( 5, 5, CV_32F );
      gaussGYKernels[ i ].create( 5, 5, CV_32F );
      createDerivGaussianKernels(  gaussGXKernels[ i ], gaussGYKernels[ i ], derivStandardDeviation);
   } 
   
   for(int i = 0; i < 5; i++ )
   {
      for(int j = 0; j < 5; j++)
      {
         cout << gaussGXKernels[0].at<float>(i,j) << " ";
      }
      cout << endl;
   }
   
   begin = clock();
   cap >> origImage1;
   //origImage1 = imread("img/graf/img1.ppm");
   origImage1.copyTo( image1Highlight);
   origImage2 = imread("img/graf/img2.ppm");
   origImage2.copyTo( image2Highlight);

   //Convert to gray scale
   origImage1.convertTo(grayImage1, CV_32F, 1/255.0);	
   cvtColor(grayImage1, grayImage1, CV_BGR2GRAY);
   
   autoCorrMat1.create( origImage1.rows, origImage1.cols, CV_32F);
   
   filter2D( grayImage1, xDeriv, -1, gaussGXKernels[ 0 ]);
   filter2D( grayImage1, yDeriv, -1, gaussGYKernels[ 0 ]);
   
   xyDeriv = xDeriv.mul(yDeriv);
   xDeriv = xDeriv.mul(xDeriv);
   yDeriv = yDeriv.mul(yDeriv);
   
   filter2D( xyDeriv, xyDeriv, -1, smoothGauss);
   filter2D( xDeriv, xDeriv, -1, smoothGauss);
   filter2D( yDeriv, yDeriv, -1, smoothGauss);
   
   createAutoCorrMatrix( grayImage1, autoCorrMat1, xyDeriv, xDeriv, yDeriv, thresholdVal );
   suppressNonMaximumsAdaptive( autoCorrMat1, featVec1, radiusSuppress, numKeep);
   findScales( grayImage1, featVec1, normLOGKernels, smoothStandardDeviation);
   
   origImage2.convertTo(grayImage2, CV_32F, 1/255.0);	
   cvtColor(grayImage2, grayImage2, CV_BGR2GRAY);
   
   autoCorrMat2.create( origImage2.rows, origImage2.cols, CV_32F);
   
   filter2D( grayImage2, xDeriv, -1, gaussGXKernels[ 0 ]);
   filter2D( grayImage2, yDeriv, -1, gaussGYKernels[ 0 ]);
   
   xyDeriv = xDeriv.mul(yDeriv);
   xDeriv = xDeriv.mul(xDeriv);
   yDeriv = yDeriv.mul(yDeriv);
   
   filter2D( xyDeriv, xyDeriv, -1, smoothGauss);
   filter2D( xDeriv, xDeriv, -1, smoothGauss);
   filter2D( yDeriv, yDeriv, -1, smoothGauss);
   
   createAutoCorrMatrix( grayImage2, autoCorrMat2, xyDeriv, xDeriv, yDeriv, thresholdVal );
   suppressNonMaximumsAdaptive( autoCorrMat2, featVec2, radiusSuppress, numKeep);
   findScales( grayImage2, featVec2, normLOGKernels, smoothStandardDeviation);

   end = clock();
   
   cout << float( end - begin) / CLOCKS_PER_SEC << endl;
	namedWindow("Orig", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Orig", 900, 0);
   namedWindow("Smoothed", WINDOW_SIZE_CHOICE);
   //createTrackbar("Threshold", "Smoothed", defaultThreshold, 50000,  updateThreshold, NULL);
    
   
   for(int i = 0; i < featVec2.size(); i++)
   {
      circle(image2Highlight, Point(featVec2.at(i).jPos,featVec2.at(i).iPos) ,  (featVec2.at(i).scale + 5) * 3, Scalar(100, 100, 0), 3);
      circle(image2Highlight, Point(featVec2.at(i).jPos,featVec2.at(i).iPos) ,  5, Scalar(0, 100, 0), -1);
   }
    
   for(;;)
   {   begin = clock();
      cap >> origImage1;
      //origImage1 = imread("img/graf/img1.ppm");
      origImage1.copyTo( image1Highlight);
      origImage1.convertTo(grayImage1, CV_32F, 1/255.0);	
      cvtColor(grayImage1, grayImage1, CV_BGR2GRAY);
      
      autoCorrMat1.create( origImage1.rows, origImage1.cols, CV_32F);
      
      filter2D( grayImage1, xDeriv, -1, gaussGXKernels[ 0 ]);
      filter2D( grayImage1, yDeriv, -1, gaussGYKernels[ 0 ]);
      
      xyDeriv = xDeriv.mul(yDeriv);
      xDeriv = xDeriv.mul(xDeriv);
      yDeriv = yDeriv.mul(yDeriv);
      
      filter2D( xyDeriv, xyDeriv, -1, smoothGauss);
      filter2D( xDeriv, xDeriv, -1, smoothGauss);
      filter2D( yDeriv, yDeriv, -1, smoothGauss);
      
      createAutoCorrMatrix( grayImage1, autoCorrMat1, xyDeriv, xDeriv, yDeriv, thresholdVal );
      suppressNonMaximumsAdaptive( autoCorrMat1, featVec1, radiusSuppress, numKeep);
      findScales( grayImage1, featVec1, normLOGKernels, smoothStandardDeviation);
   
      for(int i = 0; i < featVec1.size(); i++)
      {
         //circle(image1Highlight, Point(featVec1.at(i).jPos,featVec1.at(i).iPos) ,  (featVec1.at(i).scale + 5) * 3, Scalar(100, 100, 0), 3);
         circle(image1Highlight, Point(featVec1.at(i).jPos,featVec1.at(i).iPos) ,  5, Scalar(0, 100, 0), -1);
      }
      
      featVec1.erase(featVec1.begin(), featVec1.end());
      
      end = clock();
   
      cout << float( end - begin) / CLOCKS_PER_SEC << endl;
      
      imshow("Orig", image1Highlight);
      imshow("Smoothed", image2Highlight);
      
      if(waitKey(30) >= 0) break;
	}

	return 0;
}

void updateThreshold(int trackValue, void* userData)
{
   thresholdVal = trackValue * pow(10, -14.0);
}

