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

stitcher::stitcher( string file_name_1, string file_name_2, const unsigned int num_points = 8 )
{
   this->file_name_1 = file_name_1;
   this->file_name_2 = file_name_2;
   this->num_points = num_points;
   
   //No point in doing everything else if images don't load, so try it first
   read_images();
   
   //Setup vectors for correspondence points
   normalized_points.resize( num_points );
   raw_points.resize( num_points );
   
   raw_points[0] = stitcher_point(1.0, 1.0);
   raw_points[1] = stitcher_point(1.0, 2.0);
   raw_points[2] = stitcher_point(2.0, 2.0);
   raw_points[3] = stitcher_point(2.0, 1.0);
   raw_points[4] = stitcher_point(5.0, 5.0);
   raw_points[5] = stitcher_point(10.0, 10.0);
   raw_points[6] = stitcher_point(20.0, 10.0);
   raw_points[7] = stitcher_point(20.0, 5.0);
   
   //Setup matrices for homography
   A_matrix.create( num_points, 9, CV_64FC1 );
   H_matrix.create( 3, 3, CV_64FC1 );
   H_vector.create( 9, 1, CV_64FC1 );
   
   //initialize class members
   alpha_1 = 0.0;
   mu_x_1 = 0.0;
   mu_y_1 = 0.0;
   alpha_2 = 0.0;
   mu_x_2 = 0.0;
   mu_y_2 = 0.0;
   
   //Find the mu's and alphas for normalizing and unnormalizing the points.
   find_T_matrix_coefficients();
   
   //Normalize points to make algorithm numerically stable
   normalize_points();
   compute_H();
   //unnormalize_points(); 
   
   for(int i = 0; i < num_points / 2; i++ )
   {
      cout << (raw_points[i].col_pos * H_matrix.at<double>(0,0)) + raw_points[i].row_pos * H_matrix.at<double>(0,1) + H_matrix.at<double>(0,2) << " " <<
               raw_points[i].col_pos * H_matrix.at<double>(1,0)  + raw_points[i].row_pos * H_matrix.at<double>(1,1) + H_matrix.at<double>(1,2) << endl;
   }
   
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

void stitcher::compute_H()
{
   unsigned int curr_vec_pos = 0;
   unsigned int num_points_div_2 = num_points / 2;
   double curr_x_prime = 0.0;
   double curr_y_prime = 0.0;
   SVD svd;
   
   //Fill the A matrix with each corresponding pair of points. This assumes that the ith and
   //(ith + num_points_div_2) points in normalized points are corresponding pairs. 
   for( unsigned int curr_pair_points = 0; curr_pair_points < num_points_div_2; curr_pair_points ++ )
   {
      curr_x_prime = normalized_points[ curr_pair_points + num_points_div_2].col_pos;
      curr_y_prime = normalized_points[ curr_pair_points + num_points_div_2].row_pos;
      
      A_matrix.at<double>( curr_pair_points * 2, 0 ) = 0;
      A_matrix.at<double>( curr_pair_points * 2, 1 ) = 0;
      A_matrix.at<double>( curr_pair_points * 2, 2 ) = 0;
      
      A_matrix.at<double>( curr_pair_points * 2, 3 ) = -normalized_points[ curr_pair_points ].col_pos;
      A_matrix.at<double>( curr_pair_points * 2, 4 ) = -normalized_points[ curr_pair_points ].row_pos;
      A_matrix.at<double>( curr_pair_points * 2, 5 ) = -1;
      
      A_matrix.at<double>( curr_pair_points * 2, 6 ) =  normalized_points[ curr_pair_points ].col_pos * curr_y_prime;
      A_matrix.at<double>( curr_pair_points * 2, 7 ) =  normalized_points[ curr_pair_points ].row_pos * curr_y_prime;
      A_matrix.at<double>( curr_pair_points * 2, 8 ) =  curr_y_prime;   
      
      A_matrix.at<double>( curr_pair_points * 2 + 1, 0 ) = normalized_points[ curr_pair_points ].col_pos;
      A_matrix.at<double>( curr_pair_points * 2 + 1, 1 ) = normalized_points[ curr_pair_points ].row_pos;
      A_matrix.at<double>( curr_pair_points * 2 + 1, 2 ) = 1;
      
      A_matrix.at<double>( curr_pair_points * 2 + 1, 3 ) = 0;
      A_matrix.at<double>( curr_pair_points * 2 + 1, 4 ) = 0;
      A_matrix.at<double>( curr_pair_points * 2 + 1, 5 ) = 0;
      
      A_matrix.at<double>( curr_pair_points * 2 + 1, 6 ) =  normalized_points[ curr_pair_points ].col_pos * -curr_x_prime;
      A_matrix.at<double>( curr_pair_points * 2 + 1, 7 ) =  normalized_points[ curr_pair_points ].row_pos * -curr_x_prime;
      A_matrix.at<double>( curr_pair_points * 2 + 1, 8 ) =  -curr_x_prime;                                                
   }
   
   cout << A_matrix << endl;
   
   //Perform the SVD on the A_matrix
   svd(A_matrix, SVD::FULL_UV);
   
   for( unsigned int curr_pos = 0; curr_pos < 9; curr_pos++ )
   {
      H_vector.at<double>( curr_pos, 0 ) = svd.vt.at<double>(8, curr_pos); 
   }
   //Turn H vector into a 3x3 matrix
   for( unsigned int curr_row = 0; curr_row < 3; curr_row++ )
   {
      for( unsigned int curr_col = 0; curr_col < 3; curr_col++ )
      {
         //Divide through by last element to get proper scale
         H_matrix.at<double>( curr_row, curr_col ) = H_vector.at<double>( curr_vec_pos, 0 ) / H_vector.at<double>( 8, 0 ) ;
         curr_vec_pos++;
      }
   }
   
   cout << H_matrix << endl;
   
   //Unnormalize the H matrix so it will be correct in terms of original coordinates
}

///////////////////////
// Author: Brian Fehrman
// Calculates the mu and alpha coefficients
// needed for normalizing the correspondence points
// to make the homography algorithm numerically stable
///////////////////////

void stitcher::find_T_matrix_coefficients( )
{
   unsigned int num_points_div_2 = num_points / 2;
   
   //Find the x and y mean for the raw points
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      mu_x_1 += raw_points[ curr_point ].col_pos;
      mu_y_1 += raw_points[ curr_point ].row_pos;
      mu_x_2 += raw_points[ curr_point + num_points_div_2 ].col_pos;
      mu_y_2 += raw_points[ curr_point + num_points_div_2 ].row_pos;
   }
   
   mu_x_1 /= (double) num_points_div_2;
   mu_y_1 /= (double) num_points_div_2;
   mu_x_2 /= (double) num_points_div_2;
   mu_y_2 /= (double) num_points_div_2;
   
   //Find the alpha coefficient that is needed to scale the points to be an
   //average distance of sqrt(2.0) from the origin
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      alpha_1 += sqrt( ( raw_points[ curr_point ].col_pos - mu_x_1 ) * ( raw_points[ curr_point ].col_pos - mu_x_1 ) +
                     ( raw_points[ curr_point ].row_pos - mu_y_1 ) * ( raw_points[ curr_point ].row_pos - mu_y_1 ) );
                     
      alpha_2 += sqrt( ( raw_points[ curr_point + num_points_div_2 ].col_pos - mu_x_2 ) * ( raw_points[ curr_point + num_points_div_2 ].col_pos - mu_x_2 ) +
                     ( raw_points[ curr_point + num_points_div_2 ].row_pos - mu_y_2 ) * ( raw_points[ curr_point + num_points_div_2 ].row_pos - mu_y_2 ) );
   }
   
   alpha_1 = (num_points * sqrt( 2.0 ) ) / alpha_1;
   alpha_2 = (num_points * sqrt( 2.0 ) ) / alpha_2;
}

/////////////////////////////
// Author: Brian Fehrman
// Normalizes the correspondence points
// to make the homography algorithm
// numerically stable.
/////////////////////////////

void stitcher::normalize_points()
{
   unsigned int num_points_div_2 = num_points / 2;
   double max_p_val = 0.0;
   double max_p_prime_val = 0.0;
   
   //Normalize the raw points
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
   /*
   normalized_points[ curr_point ].col_pos = raw_points[ curr_point ].col_pos - mu_x_1;
   normalized_points[ curr_point ].row_pos = raw_points[ curr_point ].row_pos - mu_y_1;
   normalized_points[ curr_point + num_points_div_2 ].col_pos =  raw_points[ curr_point + num_points_div_2 ].col_pos - mu_x_2;
   normalized_points[ curr_point + num_points_div_2 ].row_pos = raw_points[ curr_point + num_points_div_2 ].row_pos - mu_y_2;
   
   if(abs(normalized_points[ curr_point ].col_pos) > max_p_val )
   {
      max_p_val = normalized_points[ curr_point ].col_pos;
   }
   if(abs(normalized_points[ curr_point ].row_pos) > max_p_val )
   {
      max_p_val = normalized_points[ curr_point ].row_pos;
   }
   
   if(abs(normalized_points[ curr_point + num_points_div_2 ].col_pos) > max_p_prime_val )
   {
      max_p_prime_val = normalized_points[ curr_point + num_points_div_2 ].col_pos;
   }
   if(abs(normalized_points[ curr_point + num_points_div_2 ].row_pos) > max_p_prime_val )
   {
      max_p_prime_val = normalized_points[ curr_point + num_points_div_2 ].row_pos;
   }
   */
   
   normalized_points[ curr_point ] = raw_points[ curr_point ];
   normalized_points[ curr_point + num_points_div_2  ] = raw_points[ curr_point + num_points_div_2  ];
   
   }
   
   /*
   scale_p_val = max_p_val;
   cout << "scale " << scale_p_val << endl;
   scale_p_prime_val = max_p_prime_val;
   
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      normalized_points[ curr_point ].col_pos /= scale_p_val;
      normalized_points[ curr_point ].row_pos /= scale_p_val;
      normalized_points[ curr_point + num_points_div_2 ].col_pos /= scale_p_prime_val;
      normalized_points[ curr_point + num_points_div_2 ].row_pos /= scale_p_prime_val;
   }
   * */
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
   if( read_image_file( image_1, file_name_1 ) )
   {
      cout << "Error reading image 1" << endl;
      return -1;
   }
   else
   {
      image_1.copyTo( image_1_highlight );
   }
   
   //Read in second image and make of copy of it in the image highlight
   if( read_image_file( image_2, file_name_2 ) )
   {
      cout << "Error reading image 2" << endl;
      return -1;
   }
   else
   {
      image_2.copyTo( image_2_highlight );
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
   unsigned int num_points_div_2 = num_points / 2;
   
   /*
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      normalized_points[ curr_point ].col_pos *= scale_val;
      normalized_points[ curr_point ].col_pos += mu_x_1;
       
      normalized_points[ curr_point ].row_pos *= scale_val;
      normalized_points[ curr_point ].row_pos += mu_y_1;
      
      normalized_points[ curr_point + num_points_div_2 ].col_pos *= scale_val;
      normalized_points[ curr_point + num_points_div_2 ].col_pos += mu_x_2;
      
      normalized_points[ curr_point + num_points_div_2 ].row_pos *= scale_val;
      normalized_points[ curr_point + num_points_div_2 ].row_pos += mu_y_2;
   }
   * */
   
   
   Mat T_matrix( 3, 3, CV_32F );
   Mat T_prime_matrix( 3, 3, CV_32F );

   T_matrix.at<double>( 0, 0 ) = scale_p_val;
   T_matrix.at<double>( 0, 1 ) = 0;
   T_matrix.at<double>( 0, 2 ) = -scale_p_val * mu_x_1;
   T_matrix.at<double>( 1, 0 ) = 0;
   T_matrix.at<double>( 1, 1 ) = scale_p_val;
   T_matrix.at<double>( 1, 2 ) = -scale_p_val * mu_y_1;
   T_matrix.at<double>( 2, 0 ) = 0;
   T_matrix.at<double>( 2, 1 ) = 0;
   T_matrix.at<double>( 2, 2 ) = 1;
   
   T_prime_matrix.at<double>( 0, 0 ) = scale_p_prime_val;
   T_prime_matrix.at<double>( 0, 1 ) = 0;
   T_prime_matrix.at<double>( 0, 2 ) = -scale_p_prime_val * mu_x_2;
   T_prime_matrix.at<double>( 1, 0 ) = 0;
   T_prime_matrix.at<double>( 1, 1 ) = scale_p_prime_val;
   T_prime_matrix.at<double>( 1, 2 ) = -scale_p_prime_val * mu_y_2;
   T_prime_matrix.at<double>( 2, 0 ) = 0;
   T_prime_matrix.at<double>( 2, 1 ) = 0;
   T_prime_matrix.at<double>( 2, 2 ) = 1;
   
   H_matrix = T_prime_matrix.inv() * H_matrix * T_matrix.inv();
   
}
