/**
Author: Brian Fehrman
Class: Computer Vision Fall 2012
Professor: Dr. Hoover
Assign: Program 1

This is an Feature Finding, Extracting/Describing, and Matching
program. It attempts to implement the Harris-Laplace method that is
mentioned in the Szeliski book, although I don't know that it does
so correctly. Code still needs some cleaning up. Most of the
functions are in included libraries. I am working on converting
the library functions into a class which will be much nicer.

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
   Mat auto_corr_mat_1, gray_image_1, image_1_highlighted, orig_image_1;
   Mat auto_corr_mat_2, gray_image_2, image_2_highlighted, orig_image_2;
	clock_t begin, end;
   CvScalar colors[NUM_SCALES];
   Mat combined_images, combined_images_ratio;
   const float deriv_standard_deviation = 1.5;
   vector<feat_val> feat_vec_1, feat_vec_2;
   Mat gauss_Gx_kernels[NUM_SCALES];
   Mat gauss_Gy_kernels[NUM_SCALES];
   Mat Gx, Gy, Ix, Iy, IxIy;
   vector<Mat> image_1_pyramid, image_2_pyramid;
   const int kernel_size = 5;
   vector<matches> match_vec, ratio_match_vec;
   Mat norm_LOG_kernels[NUM_SCALES];
   const int num_keep = 200;
   const int radius_suppress = 25;
   Mat smooth_gauss[NUM_SCALES];
   const float smooth_standard_deviation = 1.4;
   const float threshold_val  = 0.01;
   
   srand((unsigned)time(0));
   
   //Generate the kernels that will be used throughout the program.
   //Also create an array of colors to use for the size selection boxes.
   for(int i = 0; i < NUM_SCALES; i++)
   {
      norm_LOG_kernels[ i ].create( FEATURE_SIZE, FEATURE_SIZE, CV_32F );
      create_norm_LOG_kernel( norm_LOG_kernels[ i ],  smooth_standard_deviation + ( i *  smooth_standard_deviation ));
      
      smooth_gauss[ i ].create( kernel_size + 2, kernel_size + 2, CV_32F );
      create_gaussian_kernel( smooth_gauss[ i ], smooth_standard_deviation + ( i *  smooth_standard_deviation ) );
      
      gauss_Gx_kernels[ i ].create( kernel_size, kernel_size, CV_32F );
      gauss_Gy_kernels[ i ].create( kernel_size, kernel_size, CV_32F );
      create_deriv_gaussian_kernels(  gauss_Gx_kernels[ i ], gauss_Gy_kernels[ i ], deriv_standard_deviation);
      
      colors[i] = Scalar(rand() % 255, rand() % 255, rand() % 255 );
   } 
   
   begin = clock();
   orig_image_1 = imread("img/mine/buzzard2.jpg");
   orig_image_1.copyTo( image_1_highlighted);
   orig_image_2 = imread("img/mine/buzzard1.jpg");
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

   combined_images.copyTo(combined_images_ratio);

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

   end = clock();
   
   cout << float( end - begin) / CLOCKS_PER_SEC << endl;
   
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
         Scalar(rand() % 255, rand() % 255, rand() % 255 ), 2);
      
      circle(combined_images, Point(match_vec.at(i).j_pos_1, match_vec.at(i).i_pos_1 ) ,  5, Scalar(0, 100, 0), -1);
      circle(combined_images, Point(match_vec.at(i).j_pos_2 + orig_image_1.cols, match_vec.at(i).i_pos_2) ,  5, Scalar(0, 100, 0), -1);
   }
   
   //imwrite("img/output/Yosemite/Yosemite1_2_ratio.jpg", combined_images);
   //imwrite("img/output/g1_features.jpg", image_1_highlighted);
   //imwrite("img/output/Yosemite/Yosemite2_features.jpg", image_2_highlighted);
   
   Mat feat_zoom;
   feat_zoom.create(FEATURE_SIZE, FEATURE_SIZE*2+1 , CV_32F);
   cout << feat_zoom.cols;
   
   for( int i = 0; i < feat_zoom.rows; i++)
   {
      for( int j = 0; j < feat_zoom.cols / 2; j++ )
      {
         feat_zoom.at<float>( i, j ) = match_vec.at(1).feat_1.at<float>( i , j ) * 100;
         feat_zoom.at<float>( i, FEATURE_SIZE) = 1;
         feat_zoom.at<float>( i, j + FEATURE_SIZE +1) = match_vec.at(1).feat_2.at<float>( i , j ) *100;
      }
   }
   
   circle(combined_images, Point(match_vec.at(1).j_pos_1, match_vec.at(1).i_pos_1 ) ,  30, Scalar(0, 220, 220), 2);
   circle(combined_images, Point(match_vec.at(1).j_pos_2 +  orig_image_1.cols, match_vec.at(1).i_pos_2 ) ,  30, Scalar(0, 220, 220), 2);
   namedWindow("Zoomed", CV_WINDOW_NORMAL);
   imshow("Zoomed",feat_zoom);
   
   for(int i = 0; i < 5; i++)
   {
      pyrUp( feat_zoom, feat_zoom);
   }
   
   feat_zoom.convertTo(feat_zoom, CV_8U, 255.0);
   cvtColor(feat_zoom, feat_zoom, CV_GRAY2BGR);
   vector<int>p;
   p.push_back(CV_IMWRITE_JPEG_QUALITY);
   p.push_back(90);
   //imwrite("img/output/Yosemite/feat_zoom_3.jpg", feat_zoom, p);
   //imwrite("img/output/Yosemite/feat_zoom_3_full.jpg", combined_images, p);
    
    
   for(;;)
   {
      imshow("Orig", image_1_highlighted);
      imshow("Smoothed", image_2_highlighted);
      imshow("Combined", combined_images);
      //imshow("Combined", image_1_pyramid.at(2));
      
      if(waitKey(30) >= 0) break;
	}

	return 0;
}
