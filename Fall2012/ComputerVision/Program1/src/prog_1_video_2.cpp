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
   Mat auto_corr_mat_1, gray_image_1, image_1_highlighted, orig_image_1;
   Mat auto_corr_mat_2, gray_image_2, image_2_highlighted, orig_image_2;
	clock_t begin, end;
   CvScalar colors[100];
   Mat combined_images;
   const float deriv_standard_deviation = 1.5;
   vector<feat_val> feat_vec_1, feat_vec_2;
   Mat gauss_Gx_kernels[NUM_SCALES];
   Mat gauss_Gy_kernels[NUM_SCALES];
   Mat Gx, Gy, Ix, Iy, IxIy;
   vector<Mat> image_1_pyramid, image_2_pyramid;
   const int kernel_size = 5;
   vector<matches> match_vec;
   Mat norm_LOG_kernels[NUM_SCALES];
   const int num_keep = 40;
   const int radius_suppress = 25;
   Mat smooth_gauss[NUM_SCALES];
   const float smooth_standard_deviation = 1.4;
   VideoCapture cap(0);
   VideoWriter record_single, record_both;
   Mat tempCap;
   Size combined_size;

   srand((unsigned)time(0));
   
   for(int i = 0; i < NUM_SCALES; i++)
   {
      norm_LOG_kernels[ i ].create( FEATURE_SIZE, FEATURE_SIZE, CV_32F );
      create_norm_LOG_kernel( norm_LOG_kernels[ i ],  smooth_standard_deviation + ( i *  smooth_standard_deviation ));
      
      smooth_gauss[ i ].create( kernel_size + 2, kernel_size + 2, CV_32F );
      create_gaussian_kernel( smooth_gauss[ i ], smooth_standard_deviation + ( i *  smooth_standard_deviation ) );
      
      gauss_Gx_kernels[ i ].create( kernel_size, kernel_size, CV_32F );
      gauss_Gy_kernels[ i ].create( kernel_size, kernel_size, CV_32F );
      create_deriv_gaussian_kernels(  gauss_Gx_kernels[ i ], gauss_Gy_kernels[ i ], deriv_standard_deviation);
   } 
   
   for(int i = 0; i < num_keep; i++)
   {
       colors[i] = Scalar(rand() % 255, rand() % 255, rand() % 255 );
   }
   
   cap >> tempCap;
   
   tempCap.copyTo( orig_image_1);
   
   combined_size.height = tempCap.size().height;
   combined_size.width = tempCap.size().width * 2;
   
   record_both.open( "vid/Matching.avi", CV_FOURCC('X','V','I','D'), 10, combined_size, true);
   record_single.open( "vid/Single.avi", CV_FOURCC('X','V','I','D'), 10, tempCap.size(), true);
   
   for(;;)
   {
      match_vec.clear();
      feat_vec_1.clear();
      feat_vec_2.clear();
      begin = clock();
      cap >> tempCap;
         
      tempCap.copyTo( orig_image_2);
      
  // begin = clock();
   //orig_image_1 = imread("img/graf/img1.ppm");
   orig_image_1.copyTo( image_1_highlighted);
   //orig_image_2 = imread("img/graf/img4.ppm");
   orig_image_2.copyTo( image_2_highlighted);
   
   combined_images.create( orig_image_1.rows, orig_image_1.cols * 2, CV_8UC3);
   
   for( int i = 0; i < orig_image_1.rows; i++)
   {
      for( int j = 0; j < orig_image_1.cols; j++ )
      {
         combined_images.at<Vec3b>( i, j ) = orig_image_1.at<Vec3b>( i , j );
         combined_images.at<Vec3b>( i, j + orig_image_1.cols ) = orig_image_2.at<Vec3b>( i , j );
      }
   }

   //Convert to gray scale
   orig_image_1.convertTo(gray_image_1, CV_32F, 1/255.0);	
   cvtColor(gray_image_1, gray_image_1, CV_BGR2GRAY);
   
   buildPyramid( gray_image_1, image_1_pyramid, NUM_SCALES );
   
   auto_corr_mat_1.create( orig_image_1.rows, orig_image_1.cols, CV_32F);
   
   filter2D( gray_image_1, Ix, -1, gauss_Gx_kernels[ 0 ]);
   filter2D( gray_image_1, Iy, -1, gauss_Gy_kernels[ 0 ]);
   
   IxIy = Ix.mul(Iy);
   Ix = Ix.mul(Ix);
   Iy = Iy.mul(Iy);
   
   namedWindow("IxIy", WINDOW_SIZE_CHOICE);
   imshow( "IxIy", IxIy);
   
   filter2D( IxIy, IxIy, -1, smooth_gauss[0]);
   filter2D( Ix, Ix, -1, smooth_gauss[0]);
   filter2D( Iy, Iy, -1, smooth_gauss[0]);
   
   create_auto_corr_matrix( gray_image_1, auto_corr_mat_1, IxIy, Ix, Iy, threshold_val );
   suppress_non_maximums_adaptive( auto_corr_mat_1, feat_vec_1, radius_suppress, num_keep);
   find_scales( image_1_pyramid, feat_vec_1, norm_LOG_kernels, smooth_standard_deviation);
   extract_features( image_1_pyramid, feat_vec_1, smooth_gauss, smooth_standard_deviation, gauss_Gx_kernels[ 0 ], gauss_Gy_kernels[ 0 ] );
   
   orig_image_2.convertTo(gray_image_2, CV_32F, 1/255.0);	
   cvtColor(gray_image_2, gray_image_2, CV_BGR2GRAY);
   
   buildPyramid( gray_image_2, image_2_pyramid, NUM_SCALES );
   
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
   find_scales( image_2_pyramid, feat_vec_2, norm_LOG_kernels, smooth_standard_deviation);
   extract_features(image_2_pyramid, feat_vec_2, smooth_gauss, smooth_standard_deviation, gauss_Gx_kernels[ 0 ], gauss_Gy_kernels[ 0 ] );

   //end = clock();
   
   //cout << float( end - begin) / CLOCKS_PER_SEC << endl;
   
	namedWindow("Orig", WINDOW_SIZE_CHOICE);
	cvMoveWindow("Orig", 900, 0);
   namedWindow("Smoothed", WINDOW_SIZE_CHOICE);
   namedWindow("Combined", WINDOW_SIZE_CHOICE);

   create_feat_boxes( feat_vec_1 );
   create_feat_boxes( feat_vec_2 );
    
   for( unsigned int i = 0; i < feat_vec_1.size(); i++)
   {
      draw_boxes( image_1_highlighted, feat_vec_1.at(i).feat_box_points, colors[feat_vec_1.at(i).scale_num - 1], 2 );
      line(image_1_highlighted, Point(feat_vec_1.at(i).j_pos,feat_vec_1.at(i).i_pos),
         Point(feat_vec_1.at(i).j_pos + feat_vec_1.at(i).major_orientation_x * FEATURE_SIZE_DIV_2 * feat_vec_1.at(i).scale_num , feat_vec_1.at(i).i_pos + feat_vec_1.at(i).major_orientation_y * FEATURE_SIZE_DIV_2 * feat_vec_1.at(i).scale_num ),
         Scalar( 0, 220, 220 ), 2);
   }
   
   for( unsigned int i = 0; i < feat_vec_2.size(); i++)
   {
      draw_boxes( image_2_highlighted, feat_vec_2.at(i).feat_box_points, colors[feat_vec_2.at(i).scale_num - 1], 2 );
      line(image_2_highlighted, Point(feat_vec_2.at(i).j_pos,feat_vec_2.at(i).i_pos),
         Point(feat_vec_2.at(i).j_pos + feat_vec_2.at(i).major_orientation_x * FEATURE_SIZE_DIV_2 * feat_vec_2.at(i).scale_num, feat_vec_2.at(i).i_pos + feat_vec_2.at(i).major_orientation_y * FEATURE_SIZE_DIV_2 * feat_vec_2.at(i).scale_num),
         Scalar( 0, 220, 220 ), 2);

   }
    
   find_matches( gray_image_2.rows, gray_image_2.cols, feat_vec_1, feat_vec_2, match_vec, .16);
   
   for( unsigned int i = 0; i < match_vec.size(); i++ )
   {
      line( combined_images, Point(match_vec.at(i).j_pos_1, match_vec.at(i).i_pos_1 ),
         Point(match_vec.at(i).j_pos_2 + orig_image_1.cols, match_vec.at(i).i_pos_2),
         colors[i], 2);
      
      circle(combined_images, Point(match_vec.at(i).j_pos_1, match_vec.at(i).i_pos_1 ) ,  5, Scalar(0, 100, 0), -1);
      circle(combined_images, Point(match_vec.at(i).j_pos_2 + orig_image_1.cols, match_vec.at(i).i_pos_2) ,  5, Scalar(0, 100, 0), -1);
   }
    
      record_single << image_1_highlighted;
      record_both << combined_images;
      imshow("Orig", image_1_highlighted);
      imshow("Smoothed", image_2_highlighted);
      imshow("Combined", combined_images);
      //imshow("Combined", image_1_pyramid.at(2));
      
      orig_image_2.copyTo( orig_image_1 );
      if(waitKey(30) >= 0) break;
	}
	return 0;
}
