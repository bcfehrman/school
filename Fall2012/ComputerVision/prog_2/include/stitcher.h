/*
 * stitcher.h
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 *  
 */


#ifndef STITCHER_H
#define STITCHER_H

#include <ctime>
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/features2d/features2d.hpp>


using namespace std;
using namespace cv;

#define NUM_POINTS 8

class stitcher
{
   public:
      stitcher();
      stitcher( string file_name_1, string file_name_2, const unsigned int num_points = 8 );
      virtual ~stitcher();
   
   private:
      //Private members
      Mat A_matrix, H_matrix, H_vector;
      string file_name_1, file_name_2;
      Mat image_1, image_1_highlight;
      Mat image_2, image_2_highlight;
      float alpha_1, mu_x_1, mu_y_1;
      float alpha_2, mu_x_2, mu_y_2;
      vector<Point> normalized_points, raw_points;
      unsigned int num_points;
      
      //Private methods
      void compute_H( );
      void find_T_matrix_coefficients( );
      void normalize_points( );
      int read_image_file( Mat& dst_image, string file_name );
      int read_images();
      void unnormalize_points( );
};

#endif /* STITCHER_H */ 
