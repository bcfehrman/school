/*
 * stitcher.cpp
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 */


#include "stitcher.h"

/////////// Stitcher Constructors and Destructors /////////////

stitcher::stitcher()
{
   
}
////////////////////////////
// Author: Brian Fehrman
// Main constructor for the stitcher class
////////////////////////

stitcher::stitcher( string p_file_name, string p_prime_file_name, const unsigned int num_points)
{
   this->p_file_name = p_file_name;
   this->p_prime_file_name = p_prime_file_name;
   this->num_points = num_points;
   
   num_points_div_2 = num_points / 2;
   
   //No point in doing everything else if images don't load, so try it first
   read_images();
   
   //Determine which image coordinates are largest and use that
   //information when creating the combined image and other tasks
   if(p_image.rows > p_prime_image.rows)
   {
      image_y_max = p_image.rows;
   }
   else
   {
      image_y_max = p_prime_image.rows;
   }
   
   if(p_image.cols > p_prime_image.cols)
   {
      image_x_max = p_image.cols;
   }
   else
   {
      image_x_max = p_prime_image.cols;
   }
   
   //Setup vectors for correspondence points
   p_normalized_points.resize( num_points_div_2 );
   p_prime_normalized_points.resize( num_points_div_2 );
   p_raw_points.resize( num_points );
   p_prime_raw_points.resize( num_points );
   
   //Setup matrices for homography
   A_matrix.create( num_points, 9, CV_64FC1 );
   H_matrix.create( 3, 3, CV_64FC1 );
   H_vector.create( 9, 1, CV_64FC1 );
   
   //initialize class members
   p_alpha = 0.0;
   p_mu_x = 0.0;
   p_mu_y = 0.0;
   p_prime_alpha = 0.0;
   p_prime_mu_x = 0.0;
   p_prime_mu_y = 0.0;
}

stitcher::~stitcher()
{

}

////////// Stitcher Methods ////////////////////

/////////////////////////
// Author: Brian Fehrman
// Computes the H transform matrix
// given pairs of normalized points that are
// stored in a single vector with the pairs given by
// the ith and (ith + num_points_div_2) points in normalized 
// points vector
////////////////////////

void stitcher::compute_H(vector<Vec3d> chosen_p_points, vector<Vec3d> chosen_p_prime_points, Mat& dst_mat)
{   
   unsigned int curr_vec_pos = 0;
   double curr_x_prime = 0.0;
   double curr_y_prime = 0.0;
   SVD svd;

   for(unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      p_raw_points[ curr_point ] = chosen_p_points[ curr_point ];
      p_prime_raw_points[ curr_point ] = chosen_p_prime_points[ curr_point ];
   }

   //Normalize points to make algorithm numerically stable
   normalize_points();
   
   //Fill the A matrix with each corresponding pair of points. This assumes that the ith and
   //(ith + num_points_div_2) points in normalized points are corresponding pairs. 
   for( unsigned int curr_pair_points = 0, curr_mat_row = 0; curr_pair_points < num_points_div_2; curr_pair_points++, curr_mat_row += 2 )
   {
      curr_x_prime = p_prime_normalized_points[ curr_pair_points ][0];
      curr_y_prime = p_prime_normalized_points[ curr_pair_points ][1];
      
      A_matrix.at<double>( curr_mat_row, 0 ) = 0;
      A_matrix.at<double>( curr_mat_row, 1 ) = 0;
      A_matrix.at<double>( curr_mat_row, 2 ) = 0;
      
      A_matrix.at<double>( curr_mat_row, 3 ) = -p_normalized_points[ curr_pair_points ][0];
      A_matrix.at<double>( curr_mat_row, 4 ) = -p_normalized_points[ curr_pair_points ][1];
      A_matrix.at<double>( curr_mat_row, 5 ) = -1;
      
      A_matrix.at<double>( curr_mat_row, 6 ) =  p_normalized_points[ curr_pair_points ][0] * curr_y_prime;
      A_matrix.at<double>( curr_mat_row, 7 ) =  p_normalized_points[ curr_pair_points ][1] * curr_y_prime;
      A_matrix.at<double>( curr_mat_row, 8 ) =  curr_y_prime;   
      
      A_matrix.at<double>( curr_mat_row + 1, 0 ) = p_normalized_points[ curr_pair_points ][0];
      A_matrix.at<double>( curr_mat_row + 1, 1 ) = p_normalized_points[ curr_pair_points ][1];
      A_matrix.at<double>( curr_mat_row + 1, 2 ) = 1;
      
      A_matrix.at<double>( curr_mat_row + 1, 3 ) = 0;
      A_matrix.at<double>( curr_mat_row + 1, 4 ) = 0;
      A_matrix.at<double>( curr_mat_row + 1, 5 ) = 0;
      
      A_matrix.at<double>( curr_mat_row + 1, 6 ) =  p_normalized_points[ curr_pair_points ][0] * -curr_x_prime;
      A_matrix.at<double>( curr_mat_row + 1, 7 ) =  p_normalized_points[ curr_pair_points ][1] * -curr_x_prime;
      A_matrix.at<double>( curr_mat_row + 1, 8 ) =  -curr_x_prime;                                                
   }
   
   //Perform the SVD on the A_matrix, use full flag to get null space vector
   svd(A_matrix, SVD::FULL_UV);
   
   //Turn the null space vector into the 3x3 matrix H matrix
   for( unsigned int curr_row = 0; curr_row < 3; curr_row++ )
   {
      for( unsigned int curr_col = 0; curr_col < 3; curr_col++ )
      {
         //Divide through by last element to get proper scale
         H_matrix.at<double>( curr_row, curr_col ) = svd.vt.at<double>( 8, curr_vec_pos ) / svd.vt.at<double>( 8, 8 );
         curr_vec_pos++;
      }
   }
   
   //Unnormalize the H matrix so it will be correct in terms of original coordinates
   unnormalize_points(); 

   H_matrix.copyTo(dst_mat);
   
   /* Leave in for debugging
   for(unsigned int i = 0; i < num_points / 2; i++ )
   {
      double temp = p_raw_points[i][0] * H_matrix.at<double>(2,0)  + p_raw_points[i][1] * H_matrix.at<double>(2,1) + H_matrix.at<double>(2,2);
      
      cout << (p_raw_points[i][0] * H_matrix.at<double>(0,0) + p_raw_points[i][1] * H_matrix.at<double>(0,1) + H_matrix.at<double>(0,2))/temp << " " <<
               (p_raw_points[i][0] * H_matrix.at<double>(1,0)  + p_raw_points[i][1] * H_matrix.at<double>(1,1) + H_matrix.at<double>(1,2))/temp << endl;
   }
   * */
}

///////////////////////
// Author: Brian Fehrman
// Calculates the mu and alpha coefficients
// needed for normalizing the correspondence points
// to make the homography algorithm numerically stable
///////////////////////

void stitcher::find_T_matrix_coefficients( )
{
   p_alpha = 0.0;
   p_mu_x = 0.0;
   p_mu_y = 0.0;
   p_prime_alpha = 0.0;
   p_prime_mu_x = 0.0;
   p_prime_mu_y = 0.0;
   
   //Find the x and y mean for the raw points
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      p_mu_x += p_raw_points[ curr_point ][0];
      p_mu_y += p_raw_points[ curr_point ][1];
      p_prime_mu_x += p_prime_raw_points[ curr_point ][0];
      p_prime_mu_y += p_prime_raw_points[ curr_point ][1];
   }
   
   //Finalize the mu values
   p_mu_x /= (double) num_points_div_2;
   p_mu_y /= (double) num_points_div_2;
   p_prime_mu_x /= (double) num_points_div_2;
   p_prime_mu_y /= (double) num_points_div_2;
   
   //Find the alpha coefficient that is needed to scale the points to be an
   //average distance of sqrt(2.0) from the origin
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      p_alpha += sqrt( ( p_raw_points[ curr_point ][0] - p_mu_x ) * ( p_raw_points[ curr_point ][0] - p_mu_x ) +
                     ( p_raw_points[ curr_point ][1] - p_mu_y ) * ( p_raw_points[ curr_point ][1] - p_mu_y ) );
                     
      p_prime_alpha += sqrt( ( p_prime_raw_points[ curr_point ][0] - p_prime_mu_x ) * ( p_prime_raw_points[ curr_point ][0] - p_prime_mu_x ) +
                     ( p_prime_raw_points[ curr_point ][1] - p_prime_mu_y) * ( p_prime_raw_points[ curr_point ][1] - p_prime_mu_y ) );
   }
   
   //Finalize the alpha values
   p_alpha = (num_points_div_2 * sqrt( 2.0 ) ) / p_alpha;
   p_prime_alpha = (num_points_div_2 * sqrt( 2.0 ) ) / p_prime_alpha;
}

/////////////////////////////
// Author: Brian Fehrman
// Normalizes the correspondence points
// to make the homography algorithm
// numerically stable.
/////////////////////////////

void stitcher::normalize_points()
{
   //Find the mu's and alphas for normalizing and unnormalizing the points.
   find_T_matrix_coefficients();
   
   //Normalize the raw points
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
   
      p_normalized_points[ curr_point ][0] = p_raw_points[ curr_point ][0] - p_mu_x;
      p_normalized_points[ curr_point ][0] *= p_alpha;
      p_normalized_points[ curr_point ][1] = p_raw_points[ curr_point ][1] - p_mu_y;
      p_normalized_points[ curr_point ][1] *= p_alpha;
      
      p_prime_normalized_points[ curr_point ][0] =  p_prime_raw_points[ curr_point ][0] - p_prime_mu_x;
      p_prime_normalized_points[ curr_point ][0] *= p_prime_alpha;
      p_prime_normalized_points[ curr_point ][1] = p_prime_raw_points[ curr_point ][1] - p_prime_mu_y;
      p_prime_normalized_points[ curr_point ][1] *= p_prime_alpha;
   }
}

////////////////////
// Author: Brian Fehrman
// Reads in image 1 and image 2 which will be used by any
// of the inheriting stitching methods ( e.g., auto, manual ).
// Performs error checking for each image load, outputs an error
// and returns -1 if an error is encountered.
///////////////////

int stitcher::read_images()
{
   //Read in first image and make of copy of it in the image highlight
   if( read_image_file( p_image, p_file_name ) )
   {
      cout << "Error reading image 1" << endl;
      return -1;
   }
   else
   {
      p_image.copyTo( p_image_highlight );
   }
   
   //Read in second image and make of copy of it in the image highlight
   if( read_image_file( p_prime_image, p_prime_file_name ) )
   {
      cout << "Error reading image 2" << endl;
      return -1;
   }
   else
   {
      p_prime_image.copyTo( p_prime_image_highlight );
   }
   
   return 0;
}


////////////////////
// Author: Brian Fehrman
// Reads in an image file and stores it in the destination Mat
// that is passed in. Performs error checking and returns -1
// if the file image load fails.
////////////////////

int stitcher::read_image_file(Mat& dst_image, string file_name)
{
   //read in image
   dst_image = imread(file_name);
   
   //Check if it succeddeeded and return -1 if not
   if( !dst_image.data )
   {
      cout << "stitcher::read_image_file could not open " << file_name << ", exiting... " << endl;
      return -1;
   }
   
   return 0;
}

///////////////////////////////
// Author: Brian Fehrman
// Unormalizes the H matrix to get points
// to be in the correct coordinate scale.
///////////////////////////////

void stitcher::unnormalize_points()
{
   Mat T_matrix( 3, 3, CV_64FC1 );
   Mat T_prime_matrix( 3, 3, CV_64FC1 );

   //Create the T transform matrix using the previously found
   //alpha and mu's for p
   T_matrix.at<double>( 0, 0 ) = p_alpha;
   T_matrix.at<double>( 0, 1 ) = 0;
   T_matrix.at<double>( 0, 2 ) = -p_alpha * p_mu_x;
   T_matrix.at<double>( 1, 0 ) = 0;
   T_matrix.at<double>( 1, 1 ) = p_alpha;
   T_matrix.at<double>( 1, 2 ) = -p_alpha * p_mu_y;
   T_matrix.at<double>( 2, 0 ) = 0;
   T_matrix.at<double>( 2, 1 ) = 0;
   T_matrix.at<double>( 2, 2 ) = 1;
   
   //Create the T' transform matrix using the previously found
   //alpha and mu's for p'
   T_prime_matrix.at<double>( 0, 0 ) = p_prime_alpha;
   T_prime_matrix.at<double>( 0, 1 ) = 0;
   T_prime_matrix.at<double>( 0, 2 ) = -p_prime_alpha * p_prime_mu_x;
   T_prime_matrix.at<double>( 1, 0 ) = 0;
   T_prime_matrix.at<double>( 1, 1 ) = p_prime_alpha;
   T_prime_matrix.at<double>( 1, 2 ) = -p_prime_alpha* p_prime_mu_y;
   T_prime_matrix.at<double>( 2, 0 ) = 0;
   T_prime_matrix.at<double>( 2, 1 ) = 0;
   T_prime_matrix.at<double>( 2, 2 ) = 1;
   
   //Unnormalize H using the equation given in the slides
   H_matrix = T_prime_matrix.inv() * H_matrix * T_matrix;
}
