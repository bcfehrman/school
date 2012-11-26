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

stitcher::stitcher( string p_file_name, string p_prime_file_name, const unsigned int num_points_get, string output_mosaic_name )
{
   this->p_file_name = p_file_name;
   this->p_prime_file_name = p_prime_file_name;
   this->num_points_get = num_points_get;
   this->output_mosaic_name = output_mosaic_name;
   
   //num_points_div_2 = num_points / 2;
   
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
   p_normalized_points.resize( num_points_get);
   p_prime_normalized_points.resize( num_points_get );
   p_raw_points.resize( num_points_get );
   p_prime_raw_points.resize( num_points_get );
   
   //Setup matrices for homography
   A_matrix.create( num_points * 2, 9, CV_64FC1 );
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

void stitcher::compute_H(const vector<Vec3d>& chosen_p_points, const vector<Vec3d>& chosen_p_prime_points, Mat& dst_mat)
{   
   unsigned int curr_vec_pos = 0;
   double curr_x_prime = 0.0;
   double curr_y_prime = 0.0;
   SVD svd;

   num_points = chosen_p_points.size();
   num_points_div_2 = num_points / 2.0;
   
   p_raw_points.resize( num_points );
   p_prime_raw_points.resize( num_points );
   
   A_matrix.create( num_points * 2, 9, CV_64FC1 );

   for(unsigned int curr_point = 0; curr_point < num_points; curr_point++ )
   {
      p_raw_points[ curr_point ] = chosen_p_points[ curr_point ];
      p_prime_raw_points[ curr_point ] = chosen_p_prime_points[ curr_point ];
   }

   //Normalize points to make algorithm numerically stable
   normalize_points();

   //Fill the A matrix with each corresponding pair of points. 
   for( unsigned int curr_pair_points = 0, curr_mat_row = 0; curr_pair_points < num_points; curr_pair_points++, curr_mat_row += 2 )
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
}

///////////////////////////
// Author: Brian Fehrman
// Creates a mosaic given a p and p_prime image. Assumes P image is to the
// left and P_prime is to the right (this may not be a needed assumption)
// Assumes H matrix takes p to p_prime.
///////////////////////////
void stitcher::create_mosaic( Mat& dst_mat)
{
   double curr_tran_x = 0.0, curr_tran_y = 0.0;
   Mat scaled_mat;
   Mat H_matrix_inv = H_matrix.inv();
   double scale_val = 0.0;
   Vec3d extrema;
   Vec3d point_offsets;
   Mat temp_mat;
   
   find_extrema( extrema, point_offsets );
   
   temp_mat.create( (int) extrema[1], (int) extrema[0], CV_8UC3 );

   for( int curr_row = 0; curr_row < temp_mat.rows; curr_row++)
   {
      for( int curr_col = 0; curr_col < temp_mat.cols; curr_col++)
      {
         temp_mat.at<Vec3b>( curr_row, curr_col) = Vec3b( 0, 0, 0);
      }
   }

   for(double curr_x = 0; curr_x < p_prime_image.cols - 1; curr_x += 1 )
   {   
      for( double curr_y = 0; curr_y < p_prime_image.rows - 1; curr_y += 1 )
      {
         scale_val = curr_x * H_matrix_inv.at<double>(2,0)  + curr_y * H_matrix_inv.at<double>(2,1) + H_matrix_inv.at<double>(2,2);
         curr_tran_x = (curr_x * H_matrix_inv.at<double>(0,0) + curr_y * H_matrix_inv.at<double>(0,1) + H_matrix_inv.at<double>(0,2))/scale_val;
         curr_tran_y = (curr_x * H_matrix_inv.at<double>(1,0) + curr_y * H_matrix_inv.at<double>(1,1) + H_matrix_inv.at<double>(1,2))/scale_val;
         
         if( curr_tran_y > 0 && curr_tran_x > 0 && curr_tran_y < temp_mat.rows && curr_tran_x < temp_mat.cols)
         temp_mat.at<Vec3b>( curr_tran_y, curr_tran_x) = p_prime_image.at<Vec3b>( curr_y, curr_x );
   
      }
   }
   
   for(double curr_x = 0; curr_x < temp_mat.cols - 1; curr_x += 1 )
   {   
      for( double curr_y = 0; curr_y < temp_mat.rows - 1; curr_y += 1 )
      {
         scale_val = curr_x * H_matrix.at<double>(2,0)  + curr_y * H_matrix.at<double>(2,1) + H_matrix.at<double>(2,2);
         curr_tran_x = (curr_x * H_matrix.at<double>(0,0) + curr_y * H_matrix.at<double>(0,1) + H_matrix.at<double>(0,2))/scale_val;
         curr_tran_y = (curr_x * H_matrix.at<double>(1,0) + curr_y * H_matrix.at<double>(1,1) + H_matrix.at<double>(1,2))/scale_val;
         
         if( curr_tran_y > 0 && curr_tran_x > 0 && curr_tran_y < p_prime_image.rows && curr_tran_x < p_prime_image.cols)
            temp_mat.at<Vec3b>( curr_y, curr_x) = p_prime_image.at<Vec3b>( curr_tran_y, curr_tran_x );
   
      }
   }

   for(double curr_x = 0; curr_x < p_image.cols - 1; curr_x += 1 )
   {   
      for( double curr_y = 0; curr_y < p_image.rows - 1; curr_y += 1 )
      {
         temp_mat.at<Vec3b>( curr_y, curr_x) = p_image.at<Vec3b>( curr_y, curr_x );
      }
   }

   temp_mat.copyTo( dst_mat);
   
   imwrite(output_mosaic_name, temp_mat);
}

///////////////////////////////
// Author: Brian Fehrman
// Finds extrema needed for mosaic output image
///////////////////////////////

void stitcher::find_extrema( Vec3d& extrema, Vec3d& point_offsets )
{
   double curr_tran_x = 0.0, curr_tran_y = 0.0;
   Mat H_matrix_inv = H_matrix.inv();
   double max_x = -10000.0, max_y = -10000.0, min_x = 10000.0, min_y = 10000.0;
   double scale_val = 0.0;
   
   
   for(double curr_x = 0; curr_x < p_prime_image.cols - 1; curr_x += 1 )
   {   
      for( double curr_y = 0; curr_y < p_prime_image.rows - 1; curr_y += 1 )
      {
         scale_val = curr_x * H_matrix_inv.at<double>(2,0)  + curr_y * H_matrix_inv.at<double>(2,1) + H_matrix_inv.at<double>(2,2);
         curr_tran_x = (curr_x * H_matrix_inv.at<double>(0,0) + curr_y * H_matrix_inv.at<double>(0,1) + H_matrix_inv.at<double>(0,2))/scale_val;
         curr_tran_y = (curr_x * H_matrix_inv.at<double>(1,0) + curr_y * H_matrix_inv.at<double>(1,1) + H_matrix_inv.at<double>(1,2))/scale_val;
         
         if(curr_tran_x > max_x )
         {
            max_x = curr_tran_x;
         }
         if(curr_tran_x < min_x )
         {
            min_x = curr_tran_x;
         }
         
         if(curr_tran_y > max_y )
         {
            max_y = curr_tran_y;
         }
         if(curr_tran_y < min_y )
         {
            min_y = curr_tran_y;
         }
      }
   }
   
   /*
   if( p_image.cols > max_x )
   {
      max_x = p_image.cols;
   }
   if( 0 < min_x )
   {
      min_x = 0;
   }
   if( p_image.rows > max_y )
   {
      max_y = p_image.rows;
   }
   if( 0 < min_y )
   {
      min_y = 0;
   } 
   * 
   * */  
   
   if(max_x > 6000)
   {
    max_x = 6000;  
   }
   if(max_y > 6000)
   {
    max_y = 6000;  
   }
   
   extrema[0] = (double) max_x;
   extrema[1] = (double) max_y;
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
   for( unsigned int curr_point = 0; curr_point < num_points; curr_point++ )
   {
      p_mu_x += p_raw_points[ curr_point ][0];
      p_mu_y += p_raw_points[ curr_point ][1];
      p_prime_mu_x += p_prime_raw_points[ curr_point ][0];
      p_prime_mu_y += p_prime_raw_points[ curr_point ][1];
   }
   
   //Finalize the mu values
   p_mu_x /= (double) num_points;
   p_mu_y /= (double) num_points;
   p_prime_mu_x /= (double) num_points;
   p_prime_mu_y /= (double) num_points;
   
   //Find the alpha coefficient that is needed to scale the points to be an
   //average distance of sqrt(2.0) from the origin
   for( unsigned int curr_point = 0; curr_point < num_points; curr_point++ )
   {
      p_alpha += sqrt( ( p_raw_points[ curr_point ][0] - p_mu_x ) * ( p_raw_points[ curr_point ][0] - p_mu_x ) +
                     ( p_raw_points[ curr_point ][1] - p_mu_y ) * ( p_raw_points[ curr_point ][1] - p_mu_y ) );
                     
      p_prime_alpha += sqrt( ( p_prime_raw_points[ curr_point ][0] - p_prime_mu_x ) * ( p_prime_raw_points[ curr_point ][0] - p_prime_mu_x ) +
                     ( p_prime_raw_points[ curr_point ][1] - p_prime_mu_y) * ( p_prime_raw_points[ curr_point ][1] - p_prime_mu_y ) );
   }
   
   //Finalize the alpha values
   p_alpha = (num_points* sqrt( 2.0 ) ) / p_alpha;
   p_prime_alpha = (num_points * sqrt( 2.0 ) ) / p_prime_alpha;
}

/////////////////////////////
// Author: Brian Fehrman
// Normalizes the correspondence points
// to make the homography algorithm
// numerically stable.
/////////////////////////////

void stitcher::normalize_points()
{
   p_normalized_points.resize( num_points);
   p_prime_normalized_points.resize( num_points );
   
   //Find the mu's and alphas for normalizing and unnormalizing the points.
   find_T_matrix_coefficients();
   //Normalize the raw points
   for( unsigned int curr_point = 0; curr_point < num_points; curr_point++ )
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
