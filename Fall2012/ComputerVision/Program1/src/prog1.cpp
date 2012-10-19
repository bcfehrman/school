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
float threshold_val = .01;

/******** Main ***********/

int main( int argc, char *argv[])
{	
	clock_t begin;
	clock_t end;
	Size frame_size;
   Mat orig_image_1, image_1_highlighted;
   Mat orig_image_2, image_2_highlighted;
   Mat norm_LOG_kernels[NUM_SCALES];
   Mat gauss_Gx_kernels[NUM_SCALES];
   Mat gauss_Gy_kernels[NUM_SCALES];
   Mat gray_image_1, gray_image_2;
   Mat Gx, Ix, Gy, Iy, IxIy;
   Mat auto_corr_mat_1, auto_corr_mat_2;
   const float deriv_standard_deviation = 1.4;
   const float smooth_standard_deviation = 1.6;
   vector<feat_val> feat_vec_1, feat_vec_2;
   const int num_keep = 100;
   const int radius_suppress = 25;
   const int kernel_size = 5;
   Mat smooth_gauss[NUM_SCALES];
  
   for(int i = 0; i < NUM_SCALES; i++)
   {
      norm_LOG_kernels[ i ].create( kernel_size, kernel_size, CV_32F );
      create_norm_LOG_kernel( norm_LOG_kernels[ i ],  smooth_standard_deviation + i );
      
      smooth_gauss[ i ].create( kernel_size, kernel_size, CV_32F );
      create_gaussian_kernel( smooth_gauss[ i ], smooth_standard_deviation + i );
      
      gauss_Gx_kernels[ i ].create( kernel_size, kernel_size, CV_32F );
      gauss_Gy_kernels[ i ].create( kernel_size, kernel_size, CV_32F );
      create_deriv_gaussian_kernels(  gauss_Gx_kernels[ i ], gauss_Gy_kernels[ i ], deriv_standard_deviation);
   } 
   
   for(int i = 0; i < 5; i++ )
   {
      for(int j = 0; j < 5; j++)
      {
         cout << gauss_Gx_kernels[0].at<float>(i,j) << " ";
      }
      cout << endl;
   }
   
   begin = clock();
   orig_image_1 = imread("img/Yosemite/Yosemite1.jpg");
   orig_image_1.copyTo( image_1_highlighted);
   orig_image_2 = imread("img/Yosemite/Yosemite2.jpg");
   orig_image_2.copyTo( image_2_highlighted);

   //Convert to gray scale
   orig_image_1.convertTo(gray_image_1, CV_32F, 1/255.0);	
   cvtColor(gray_image_1, gray_image_1, CV_BGR2GRAY);
   
   auto_corr_mat_1.create( orig_image_1.rows, orig_image_1.cols, CV_32F);
   
   filter2D( gray_image_1, Ix, -1, gauss_Gx_kernels[ 0 ]);
   filter2D( gray_image_1, Iy, -1, gauss_Gy_kernels[ 0 ]);
   
   IxIy = Ix.mul(Iy);
   Ix = Ix.mul(Ix);
   Iy = Iy.mul(Iy);
   
   filter2D( IxIy, IxIy, -1, smooth_gauss[0]);
   filter2D( Ix, Ix, -1, smooth_gauss[0]);
   filter2D( Iy, Iy, -1, smooth_gauss[0]);
   
   create_auto_corr_matrix( gray_image_1, auto_corr_mat_1, IxIy, Ix, Iy, threshold_val );
   suppress_non_maximums_adaptive( auto_corr_mat_1, feat_vec_1, radius_suppress, num_keep);
   find_scales( gray_image_1, feat_vec_1, norm_LOG_kernels, smooth_standard_deviation);
   extract_features( gray_image_1, feat_vec_1, smooth_gauss, smooth_standard_deviation );
   find_orientations( gray_image_1, feat_vec_1, gauss_Gx_kernels[ 0 ], gauss_Gy_kernels[ 0 ]);
   
   orig_image_2.convertTo(gray_image_2, CV_32F, 1/255.0);	
   cvtColor(gray_image_2, gray_image_2, CV_BGR2GRAY);
   
   auto_corr_mat_2.create( orig_image_2.rows, orig_image_2.cols, CV_32F);
   
   filter2D( gray_image_2, Ix, -1, gauss_Gx_kernels[ 0 ]);
   filter2D( gray_image_2, Iy, -1, gauss_Gy_kernels[ 0 ]);
   
   IxIy = Ix.mul(Iy);
   Ix = Ix.mul(Ix);
   Iy = Iy.mul(Iy);
   
   filter2D( IxIy, IxIy, -1, smooth_gauss[0]);
   filter2D( Ix, Ix, -1, smooth_gauss[0]);
   filter2D( Iy, Iy, -1, smooth_gauss[0]);
   
   create_auto_corr_matrix( gray_image_2, auto_corr_mat_2, IxIy, Ix, Iy, threshold_val );
   suppress_non_maximums_adaptive( auto_corr_mat_2, feat_vec_2, radius_suppress, num_keep);
   find_scales( gray_image_2, feat_vec_2, norm_LOG_kernels, smooth_standard_deviation);
   extract_features( gray_image_2, feat_vec_2, smooth_gauss, smooth_standard_deviation );
   find_orientations( gray_image_2, feat_vec_2, gauss_Gx_kernels[ 0 ], gauss_Gy_kernels[ 0 ]);

   end = clock();
   
   cout << float( end - begin) / CLOCKS_PER_SEC << endl;
	namedWindow("Orig", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Orig", 900, 0);
   namedWindow("Smoothed", WINDOW_SIZE_CHOICE);
    
   for( unsigned int i = 0; i < feat_vec_1.size(); i++)
   {
      circle(image_1_highlighted, Point(feat_vec_1.at(i).j_pos,feat_vec_1.at(i).i_pos) ,  (feat_vec_1.at(i).scale + 5) * 3, Scalar(100, 100, 0), 3);
      circle(image_1_highlighted, Point(feat_vec_1.at(i).j_pos,feat_vec_1.at(i).i_pos) ,  5, Scalar(0, 100, 0), -1);
      line(image_1_highlighted, Point(feat_vec_1.at(i).j_pos,feat_vec_1.at(i).i_pos),
         Point(feat_vec_1.at(i).j_pos + feat_vec_1.at(i).major_orientation_x * 20, feat_vec_1.at(i).i_pos + feat_vec_1.at(i).major_orientation_y * 20),
         Scalar( 0, 0, 100 ), 2);
   }
   
   for( unsigned int i = 0; i < feat_vec_2.size(); i++)
   {
      circle(image_2_highlighted, Point(feat_vec_2.at(i).j_pos,feat_vec_2.at(i).i_pos) ,  (feat_vec_2.at(i).scale + 5) * 3, Scalar(100, 100, 0), 3);
      circle(image_2_highlighted, Point(feat_vec_2.at(i).j_pos,feat_vec_2.at(i).i_pos) ,  5, Scalar(0, 100, 0), -1);
      line(image_2_highlighted, Point(feat_vec_2.at(i).j_pos,feat_vec_2.at(i).i_pos),
         Point(feat_vec_2.at(i).j_pos + feat_vec_2.at(i).major_orientation_x * 20, feat_vec_2.at(i).i_pos + feat_vec_2.at(i).major_orientation_y * 20),
         Scalar( 0, 0, 100 ), 2);
   }
    
   for(;;)
   {
      imshow("Orig", image_1_highlighted);
      imshow("Smoothed", image_2_highlighted);
      
      if(waitKey(30) >= 0) break;
	}

	return 0;
}
