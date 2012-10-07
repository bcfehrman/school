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

/***** Prototypes *****/
Mat createGausianKernal( double standardDeviation, int size);
Mat gaussianFilter( Mat origImage, Mat kernel );

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
	Mat gaussKernel;
	double time_diff = 0;
	xy_pos window_pos;
	double standardDeviation = 1.5;
	int kernelSize = 5;
	
	if(argc > 1)
		standardDeviation = atof(argv[1]);
	
	if(argc > 2)
		kernelSize = atoi(argv[2]);
	
	cap.open(1); //open the default camera

	//Check to see if the camera opened
	if(!cap.isOpened())
	{
	cout << "Could not open" << endl;
	return -1;
	}
	
	gaussKernel = createGausianKernal(standardDeviation, kernelSize);
	
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
   Mat FFTKern(480, 640, CV_32F);
   
   for(int i = 0; i < FFTKern.size().height; i++)
   for(int j = 0; j < FFTKern.size().width; j++)
   {
      int offsety = i - FFTKern.size().height /2;
      int offsetx = j - FFTKern.size().width /2;
      
      if( sqrt(offsety * offsety + offsetx * offsetx) > 40)
         FFTKern.at<float>(i,j) = 1;
      else
         FFTKern.at<float>(i,j) = 0;
   }
   
   int cx = FFTKern.cols/2;
int cy = FFTKern.rows/2;

Mat q0(FFTKern, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
Mat q1(FFTKern, Rect(cx, 0, cx, cy));  // Top-Right
Mat q2(FFTKern, Rect(0, cy, cx, cy));  // Bottom-Left
Mat q3(FFTKern, Rect(cx, cy, cx, cy)); // Bottom-Right

Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
q0.copyTo(tmp);
q3.copyTo(q0);
tmp.copyTo(q3);

q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
q2.copyTo(q1);
tmp.copyTo(q2);
	namedWindow("FFTKernel", WINDOW_SIZE_CHOICE);
  
	Mat frame_bgr;
	cap >> frame_bgr;
   FFTKern.convertTo(FFTKern, CV_8U, 255);
    imshow("FFT_Kernel", FFTKern);
   imwrite("FFT_Kernel.jpg", FFTKern);
      //Convert the frame t be 32bit floating
   frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);	
   
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
      if(frame_bgr.at<float>(i,j) < .38)
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
                     frame_orig.at<Vec3b>(i+k,j+l).val[1] = 0;
                     frame_orig.at<Vec3b>(i+k,j+l).val[0] = 150;
                     frame_orig.at<Vec3b>(i+k,j+l).val[2] = 0;
                  }
            }
      }
   }
   frame_bgr.convertTo(frame_bgr, CV_8U, 255.0);	
		
		//Convert to gray scale
		cvtColor(frame_bgr, frame_bgr, CV_GRAY2BGR);
            orig_edge_highlight << frame_orig;
            blur << frame_bgr;       
                   
       		imshow("Smoothed", frame_bgr);
            imshow("Color", frame_orig);

		//Will exit if a window is in focus an a key is pressed
		if(waitKey(30) >= 0) break;
	}

	return 0;
}

Mat createGausianKernal( double standardDeviation, int size)
{
	Mat kernel(size, size, CV_32F);
	int sizeDiv2 = size / 2;
	double variance = standardDeviation * standardDeviation;
	double num;
	double den = 2 * M_PI * variance;
	int x,ySqrd;
	double kernelVal = 0;
	double preNormSum = 0;
	double normSum = 0;

	for(int i = 0; i < size; i++)
	{
		ySqrd = i -sizeDiv2;
		ySqrd *= ySqrd;
		
		for(int j = 0; j < size; j++)
		{
			x = j - sizeDiv2;
		
			num =  exp(-1 * ( ( x * x + ySqrd) / ( 2 * variance ) ) );
			
			kernelVal = num / den;
			
			kernel.at<float>(i,j) = kernelVal;
			
			preNormSum += kernelVal;
		}
	}
	
	for(int i = 0; i < size; i++)
	{
		for( int j = 0; j < size; j++ )
		{
			kernel.at<float>(i,j) = kernel.at<float>(i,j) / preNormSum;
			normSum += kernel.at<float>(i,j);
		}
	}
		
	return kernel;
}

Mat gaussianFilter( Mat origImage, Mat kernel )
{
	int origRowSize = origImage.size().height;
	int origColSize = origImage.size().width;
	int kernelSize = kernel.size().width;
	int kernelSizeDiv2 = kernelSize / 2;
	int newRowSize = origRowSize - (kernelSizeDiv2 * 2);
	int newColSize = origColSize - (kernelSizeDiv2 * 2);
	
	Mat filteredImage(newRowSize, newColSize, CV_32F);
	double average = 0;
	int currRow, currCol, newRow, newCol, filtRow, filtCol, kernelRow, kernelCol;

	for(currRow = 0, newRow = 0; currRow < newRowSize; currRow++, newRow++)
	{ 
		for(currCol = 0, newCol = 0; currCol < newColSize; currCol++, newCol++)
		{
			average = 0;
		
		for(filtRow = currRow, kernelRow = 0; filtRow < (currRow +  kernelSize); filtRow++, kernelRow++)
		{
			
			for( filtCol = currCol, kernelCol = 0; filtCol < (currCol +  kernelSize); filtCol++, kernelCol++)
			{
				average += origImage.at<float>(filtRow, filtCol) * kernel.at<float>(kernelRow, kernelCol);
			}
		}
			filteredImage.at<float>(newRow, newCol) = average;
		}
	}

	return filteredImage;
}
