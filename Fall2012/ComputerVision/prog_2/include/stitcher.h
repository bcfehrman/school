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
      stitcher( string p_file_name, string p_prime_file_name, const unsigned int num_points = 8, string output_mosaic_name = "");
      virtual ~stitcher();
      
   //Private members
      Mat A_matrix, H_matrix, H_vector;
      double avg_intensity;
      string p_file_name, p_prime_file_name;
      unsigned int image_x_max, image_y_max;
      unsigned int num_points_get, num_points, num_points_div_2;
      string output_mosaic_name;
      double p_alpha, p_mu_x, p_mu_y;
      Mat p_image, p_image_highlight;
      vector<Vec3d> p_normalized_points, p_raw_points;
      double p_prime_alpha, p_prime_mu_x, p_prime_mu_y;
      Mat p_prime_image, p_prime_image_highlight;
      vector<Vec3d> p_prime_normalized_points, p_prime_raw_points;
      
   //Private methods
      void compute_H(const vector<Vec3d>& chosen_p_points, const vector<Vec3d>& chosen_p_prime_points, Mat& dst_mat);
      void create_mosaic( Mat& dst_mat);
      void find_extrema( Vec3d& extrema, Vec3d& point_offsets );
      void find_T_matrix_coefficients( );
      void normalize_points( );
      int read_image_file( Mat& dst_image, string file_name );
      int read_images();
      void unnormalize_points( );
};

#endif /* STITCHER_H */ 
