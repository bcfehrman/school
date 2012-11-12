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
   
   srand((unsigned)time(0));
   
   for(int i = 0; i < num_points; i++ )
   {
      raw_points[i].x = rand()%20;
      raw_points[i].y = rand()%20;
      
      cout << raw_points[i].x << " " << raw_points[i].y << endl;
   }
   
   //Setup matrices for homography
   A_matrix.create( num_points, 9, CV_32F );
   H_matrix.create( 3, 3, CV_32F );
   H_vector.create( 9, 1, CV_32F );
   
   //initialize class members
   alpha_1 = 0.0;
   mu_x_1 = 0.0;
   mu_y_1 = 0.0;
   alpha_2 = 0.0;
   mu_x_2 = 0.0;
   mu_y_2 = 0.0;
   
   compute_H();
   
   for(int i = 0; i < num_points; i++ )
   {
      cout << raw_points[i].x * H_matrix.at<float>(0,0) +  raw_points[i].y * H_matrix.at<float>(0,1) + H_matrix.at<float>(0,2) << " " <<
               raw_points[i].x * H_matrix.at<float>(1,0) +  raw_points[i].y * H_matrix.at<float>(1,1) + H_matrix.at<float>(1,2) << endl;
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
   float curr_x_prime = 0.0;
   float curr_y_prime = 0.0;
   SVD svd;
   
   //Find the mu's and alphas for normalizing and unnormalizing the points.
   find_T_matrix_coefficients();
   
   //Normalize points to make algorithm numerically stable
   normalize_points();
   
   //Fill the A matrix with each corresponding pair of points. This assumes that the ith and
   //(ith + num_points_div_2) points in normalized points are corresponding pairs. O parts
   //of the A matrix are taken care of in initialization by OpenCV
   for( unsigned int curr_pair_points = 0; curr_pair_points < num_points_div_2; curr_pair_points ++ )
   {
      curr_x_prime = normalized_points[ curr_pair_points + num_points_div_2].x;
      curr_y_prime = normalized_points[ curr_pair_points + num_points_div_2].y;
      
      A_matrix.at<float>( curr_pair_points * 2, 3 ) = normalized_points[ curr_pair_points ].x;
      A_matrix.at<float>( curr_pair_points * 2, 4 ) = normalized_points[ curr_pair_points ].y;
      A_matrix.at<float>( curr_pair_points * 2, 5 ) = 1;
      
      A_matrix.at<float>( curr_pair_points * 2, 6 ) =  normalized_points[ curr_pair_points ].x * -curr_y_prime;
      A_matrix.at<float>( curr_pair_points * 2, 7 ) =  normalized_points[ curr_pair_points ].y * -curr_y_prime;
      A_matrix.at<float>( curr_pair_points * 2, 8 ) =  -curr_y_prime;   
      
      A_matrix.at<float>( curr_pair_points * 2 + 1, 0 ) = normalized_points[ curr_pair_points ].x;
      A_matrix.at<float>( curr_pair_points * 2 + 1, 1 ) = normalized_points[ curr_pair_points ].y;
      A_matrix.at<float>( curr_pair_points * 2 + 1, 2 ) = 1;
      
      A_matrix.at<float>( curr_pair_points * 2 + 1, 6 ) =  normalized_points[ curr_pair_points ].x * -curr_x_prime;
      A_matrix.at<float>( curr_pair_points * 2 + 1, 7 ) =  normalized_points[ curr_pair_points ].y * -curr_x_prime;
      A_matrix.at<float>( curr_pair_points * 2 + 1, 8 ) =  -curr_x_prime;                                                
   }
   
   cout  << A_matrix << endl;
   
   //Perform the SVD on the A_matrix
   svd(A_matrix);
   
   for( unsigned int curr_pos = 0; curr_pos < 9; curr_pos++ )
   {
      cout << svd.vt.at<float>(curr_pos , 8) << endl;
      H_vector.at<float>( curr_pos, 0 ) = svd.vt.at<float>(curr_pos , 8); 
   }
   //Turn H vector into a 3x3 matrix
   for( unsigned int curr_row = 0; curr_row < 3; curr_row++ )
   {
      for( unsigned int curr_col = 0; curr_col < 3; curr_col++ )
      {
         cout << H_vector.at<float>( 8, 0 )<< endl;
         //Divide through by last element to get proper scale
         H_matrix.at<float>( curr_row, curr_col ) = H_vector.at<float>( curr_vec_pos ) ;// H_vector.at<float>( 0, 8 );
         curr_vec_pos++;
      }
   }
   
   //Unnormalize the H matrix so it will be correct in terms of original coordinates
   //unnormalize_points(); 
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
      mu_x_1 += raw_points[ curr_point ].x;
      mu_y_1 += raw_points[ curr_point ].y;
      mu_x_2 += raw_points[ curr_point + num_points_div_2 ].x;
      mu_y_2 += raw_points[ curr_point + num_points_div_2 ].y;
   }
   
   mu_x_1 /= (float) num_points_div_2;
   mu_y_1 /= (float) num_points_div_2;
   mu_x_2 /= (float) num_points_div_2;
   mu_y_2 /= (float) num_points_div_2;
   
   //Find the alpha coefficient that is needed to scale the points to be an
   //average distance of sqrt(2.0) from the origin
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      alpha_1 += sqrt( ( raw_points[ curr_point ].x - mu_x_1 ) * ( raw_points[ curr_point ].x - mu_x_1 ) +
                     ( raw_points[ curr_point ].y - mu_y_1 ) * ( raw_points[ curr_point ].y - mu_y_1 ) );
                     
      alpha_2 += sqrt( ( raw_points[ curr_point + num_points_div_2 ].x - mu_x_2 ) * ( raw_points[ curr_point + num_points_div_2 ].x - mu_x_2 ) +
                     ( raw_points[ curr_point + num_points_div_2 ].y - mu_y_2 ) * ( raw_points[ curr_point + num_points_div_2 ].y - mu_y_2 ) );
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
   
   //Normalize the raw points
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      /*normalized_points[ curr_point ].x = alpha_1 * ( raw_points[ curr_point ].x - mu_x_1 );
      normalized_points[ curr_point ].y = alpha_1 * ( raw_points[ curr_point ].y - mu_y_1 );
      normalized_points[ curr_point + num_points_div_2 ].x = alpha_2 * ( raw_points[ curr_point + num_points_div_2 ].x - mu_x_2 );
      normalized_points[ curr_point + num_points_div_2 ].y = alpha_2 * ( raw_points[ curr_point + num_points_div_2 ].y - mu_y_2 );
      */
      
      normalized_points[ curr_point ]= raw_points[curr_point ];
      normalized_points[ curr_point + num_points_div_2 ] = raw_points[curr_point + num_points_div_2];
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
   Mat T_matrix( 3, 3, CV_32F );
   Mat T_prime_matrix( 3, 3, CV_32F );

   T_matrix.at<float>( 0, 0 ) = alpha_1;
   T_matrix.at<float>( 0, 2 ) = -alpha_1 * mu_x_1;
   T_matrix.at<float>( 1, 1 ) = alpha_1;
   T_matrix.at<float>( 1, 2 ) = -alpha_1 * mu_y_1;
   T_matrix.at<float>( 2, 0 ) = 1;
   
   T_prime_matrix.at<float>( 0, 0 ) = alpha_2;
   T_prime_matrix.at<float>( 0, 2 ) = -alpha_2 * mu_x_2;
   T_prime_matrix.at<float>( 1, 1 ) = alpha_2;
   T_prime_matrix.at<float>( 1, 2 ) = -alpha_2 * mu_y_2;
   T_prime_matrix.at<float>( 2, 0 ) = 1;
   
   H_matrix = T_prime_matrix.inv() * H_matrix * T_matrix;
}
