/*
 * auto_stitch.cpp
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 * 
 */


#include "auto_stitch.h"


auto_stitch::auto_stitch()
{
   
}

auto_stitch::auto_stitch(string p_file_name, string p_prime_file_name, const unsigned int num_points, string output_mosaic_name, const unsigned int stitch_type)
   : stitcher(p_file_name, p_prime_file_name, num_points, output_mosaic_name)
{
   
}


auto_stitch::~auto_stitch()
{
   
}

int auto_stitch::run()
{
   SurfFeatureDetector p_detector;
   SurfFeatureDetector p_prime_detector;
   SurfDescriptorExtractor p_extractor, p_prime_extractor;
   FlannBasedMatcher flann_matcher;
   //BruteForceMatcher< L2<float> > flann_matcher;
   vector<DMatch> good_matches;
   Mat mosaic_image;
   vector<DMatch> point_matches;
   vector<KeyPoint> p_key_points, p_suppressed_key_points;
   vector<KeyPoint> p_prime_key_points, p_prime_suppressed_key_points;
   Mat p_descriptors, p_prime_descriptors;
   Mat temp_h_mat;
   unsigned int size_erase = 50;
   
   p_detector.detect( p_image, p_key_points);
   p_prime_detector.detect( p_prime_image, p_prime_key_points);
   
   //vector<KeyPoint> p_temp_suppress;
   //vector<KeyPoint> p_prime_temp_suppress;
   //surf_suppress_non_maximums_adaptive( p_image, p_key_points, p_temp_suppress, 60, 30 );
   
   //surf_suppress_non_maximums_adaptive( p_image, p_temp_suppress, p_suppressed_key_points, 60, 30 );
   //surf_suppress_non_maximums_adaptive( p_prime_image, p_prime_key_points, p_prime_temp_suppress, 60, 30 );
   
   //surf_suppress_non_maximums_adaptive( p_prime_image, p_prime_temp_suppress, p_prime_suppressed_key_points, 60, 30 );
   
   p_extractor.compute( p_image, p_key_points, p_descriptors);
   p_prime_extractor.compute( p_prime_image, p_prime_key_points, p_prime_descriptors);
   
   flann_matcher.match( p_descriptors, p_prime_descriptors, point_matches);

   sort( point_matches.begin(), point_matches.end(), auto_stitch::sort_match_val );
     
   if( point_matches.size() < size_erase )
   {
      size_erase = point_matches.size() - 1;
   }
   
   point_matches.erase( point_matches.begin() + size_erase, point_matches.end());
   
   srand((unsigned int) time(0) );
   //double scale_val, curr_tran_x, curr_tran_y;
   
   /*
   for( int curr_point = 0; curr_point < 10; curr_point++)
   {
      Scalar curr_color = Scalar( rand() % 255, rand() % 255, rand()%255);
      circle(p_image_highlight, p_key_points[ point_matches[curr_point].queryIdx].pt, 10, curr_color, -1);
      circle(p_prime_image_highlight, p_prime_key_points[ point_matches[curr_point].trainIdx].pt, 10, curr_color, -1);
      //chosen_p_points.push_back(Vec3d( (double)p_key_points[ point_matches[curr_point].queryIdx].pt.x,(double)p_key_points[ point_matches[curr_point].queryIdx].pt.y, 1.0 ) );
      //chosen_p_prime_points.push_back(Vec3d( (double)p_prime_key_points[ point_matches[curr_point].trainIdx].pt.x,(double)p_prime_key_points[ point_matches[curr_point].trainIdx].pt.y, 1.0 ) );
   }
   */
   //compute_H( chosen_p_points, chosen_p_prime_points, temp_h_mat);
   
   /* Debugging code
   for(int curr_point = 0; curr_point < 4; curr_point++)
   {
      scale_val = chosen_p_points[curr_point][0] * H_matrix.at<double>(2,0)  + chosen_p_points[curr_point][1]* H_matrix.at<double>(2,1) + H_matrix.at<double>(2,2);
      curr_tran_x = (chosen_p_points[curr_point][0] * H_matrix.at<double>(0,0) + chosen_p_points[curr_point][1] * H_matrix.at<double>(0,1) + H_matrix.at<double>(0,2))/scale_val;
      curr_tran_y = (chosen_p_points[curr_point][0] * H_matrix.at<double>(1,0) + chosen_p_points[curr_point][1] * H_matrix.at<double>(1,1) + H_matrix.at<double>(1,2))/scale_val;
   
      cout << chosen_p_prime_points[curr_point][0] << " " << chosen_p_prime_points[curr_point][1] << endl;
      cout << curr_tran_x << " " << curr_tran_y << endl;
   }
   * */
   
   
   ransac_auto_stitch( p_key_points, p_prime_key_points, point_matches);
   
   create_mosaic( mosaic_image );

   namedWindow( "highlighted", WINDOW_SIZE_CHOICE);
   namedWindow( "highlighted_prime", WINDOW_SIZE_CHOICE);
   namedWindow( "mosaic_image", WINDOW_SIZE_CHOICE);
   for(;;)
   {
      imshow( "highlighted", p_image_highlight );
      imshow( "highlighted_prime", p_prime_image_highlight );
      imshow( "mosaic_image", mosaic_image );
      if( (int) waitKey(30) > 0)
      {
         break;
      }
      
   }
   
   return 0;
   
}

void auto_stitch::ransac_auto_stitch(const vector<KeyPoint>& p_key_points, const vector<KeyPoint>& p_prime_key_points,
                                       const vector<DMatch>& point_matches)
{
   Mat best_H_matrix(3,3,CV_64FC1);
   int curr_num_correct = 0;
   int curr_rand_index = 0;
   int curr_q_idx = 0, curr_t_idx = 0;
   double curr_tran_x = 0.0, curr_tran_y = 0.0;
   double curr_x = 0.0, curr_y = 0.0;
   double curr_x_prime = 0.0, curr_y_prime = 0.0;
   int num_matches = point_matches.size() - 1;
   int max_num_correct = -1;
   double scale_val = 0.0;
   vector<Vec3d> chosen_p_points, chosen_p_prime_points;
   vector<int> selected_indices;
   Mat temp_h_mat;
   
   
   for(int i = 0; i < 100000; i++)
   {
      chosen_p_points.clear();
      chosen_p_prime_points.clear();
      curr_num_correct = 0;
      selected_indices.clear();
      
      for(int curr_point = 0; curr_point < 10; curr_point++ )
      {
         //Get a random index to get a random match from the matches array
         do
         {
            curr_rand_index = rand() % num_matches;

         }while( is_selected( p_key_points, p_prime_key_points, point_matches, selected_indices, curr_rand_index ) );
         
         selected_indices.push_back( curr_rand_index );
      
         
         curr_q_idx = point_matches[curr_rand_index].queryIdx;
         curr_t_idx = point_matches[curr_rand_index].trainIdx;
         
         //Get  prime coordinates for current match
         curr_x = (double) p_key_points[ curr_q_idx ].pt.x;
         curr_y = (double) p_key_points[ curr_q_idx ].pt.y;
         
         chosen_p_points.push_back( Vec3d( curr_x, curr_y, 1.0 ) );
         
         //Get p prime coordinates for current match
         curr_x_prime = (double) p_prime_key_points[ curr_t_idx ].pt.x;
         curr_y_prime = (double) p_prime_key_points[ curr_t_idx ].pt.y;
         
         chosen_p_prime_points.push_back( Vec3d( curr_x_prime, curr_y_prime, 1.0 ) );
      
      }
   
      compute_H( chosen_p_points, chosen_p_prime_points, temp_h_mat);

      for( int curr_match = 0; curr_match < num_matches; curr_match++)
      {
         curr_q_idx = point_matches [ curr_match ].queryIdx;
         curr_t_idx = point_matches [ curr_match ].trainIdx;
         
         curr_x = (double) p_key_points[ curr_q_idx ].pt.x;
         curr_y = (double) p_key_points[ curr_q_idx ].pt.y;
         
         scale_val = curr_x * H_matrix.at<double>( 2, 0 )  + curr_y * H_matrix.at<double>( 2, 1 ) + H_matrix.at<double>( 2, 2 );
         curr_tran_x = ( curr_x * H_matrix.at<double>( 0, 0 ) + curr_y * H_matrix.at<double>( 0, 1 ) + H_matrix.at<double>( 0, 2 ) ) / scale_val;
         curr_tran_y = ( curr_x * H_matrix.at<double>( 1, 0 ) + curr_y * H_matrix.at<double>( 1, 1 ) + H_matrix.at<double>( 1, 2 ) ) / scale_val;
         
         curr_x_prime = p_prime_key_points[curr_t_idx].pt.x;
      
         if( abs( curr_tran_x - curr_x_prime ) < 5 )
         {
            curr_y_prime = p_prime_key_points[curr_t_idx].pt.y;
            
            if( abs( curr_tran_y - curr_y_prime ) < 5 )
            {
               curr_num_correct++;
            }
         }  
      }
      
      if( curr_num_correct > max_num_correct )
      {
         max_num_correct = curr_num_correct;
         cout << "Max Num correct: " << max_num_correct << endl;
         H_matrix.copyTo( best_H_matrix );
      }
   } 
   

   best_H_matrix.copyTo(H_matrix);
}

bool auto_stitch::is_selected(const vector<KeyPoint>& p_key_points, const vector<KeyPoint>& p_prime_key_points,
                                       const vector<DMatch>& point_matches, const vector<int>& selected_indices, 
                                       const int curr_rand_index)
{
   int curr_q_idx = 0, curr_t_idx = 0;
   double curr_x = 0.0, curr_y = 0.0;
   double curr_check_x = 0.0, curr_check_y = 0.0;
   double curr_x_prime = 0.0, curr_y_prime = 0.0;
   double curr_check_x_prime = 0.0, curr_check_y_prime = 0.0;
   int curr_selected_idx = 0;
   int min_distance = 5;
   bool used = false;

   curr_q_idx = point_matches[curr_rand_index].queryIdx;
   curr_t_idx = point_matches[curr_rand_index].trainIdx;
   curr_check_x = (double) p_key_points[ curr_q_idx ].pt.x;
   curr_check_y = (double) p_key_points[ curr_q_idx ].pt.y;
   curr_check_x_prime = (double) p_prime_key_points[ curr_t_idx ].pt.x;
   curr_check_y_prime = (double) p_prime_key_points[ curr_t_idx ].pt.y;
   
   
   for( unsigned int curr_idx = 0; curr_idx < selected_indices.size(); curr_idx++ )
   {
      curr_selected_idx = selected_indices[ curr_idx ];
               
      curr_q_idx = point_matches[ curr_selected_idx ].queryIdx;
      curr_t_idx = point_matches[ curr_selected_idx ].trainIdx;
      
      //Get  prime coordinates for current match
      curr_x = (double) p_key_points[ curr_selected_idx ].pt.x;
      curr_y = (double) p_key_points[ curr_selected_idx ].pt.y;
      
      //Get p prime coordinates for current match
      curr_x_prime = (double) p_prime_key_points[ curr_t_idx ].pt.x;
      curr_y_prime = (double) p_prime_key_points[ curr_t_idx ].pt.y;

      //Forces at minimum pixel distance between points and inherently
      //does not allow for repeats
      if( abs( curr_check_x - curr_x ) < min_distance || abs( curr_check_y - curr_y ) < min_distance )
      {
         used = true;
         break;
      }
      else if( abs( curr_check_x_prime - curr_x_prime ) < min_distance || abs( curr_check_y_prime - curr_y_prime ) < min_distance )
      {
         used = true;
         break;
      } 
   }
   
   return used;
}

bool auto_stitch::sort_response_val( KeyPoint response_val_1, KeyPoint response_val_2 )
{
   return ( response_val_1.response > response_val_2.response );
}

//Sort function used for sorting a vector of feat_val structs
//and is based upon their intensity response value wrt the
//harrison operator.
bool auto_stitch::sort_match_val( DMatch match_val_1, DMatch match_val_2 )
{
   return ( match_val_1.distance < match_val_2.distance );
}

void auto_stitch::surf_suppress_non_maximums_adaptive( const Mat& src_mat, const vector<KeyPoint>& src_vec, vector<KeyPoint>& dst_vec, 
                                                         const int size_neighbor, const unsigned int num_keep)
{
   float curr_check = 0.0;
   bool break_out = false;
   int num_rows = src_mat.rows;
   int num_cols = src_mat.cols;
   int top_range = size_neighbor;
   int bottom_range = size_neighbor;
   int left_range = size_neighbor;
   int right_range = size_neighbor;
   Mat suppress_mat;
   KeyPoint curr_feat;
   
   suppress_mat.create( num_rows, num_cols, CV_64FC1);
   
   for(int curr_row = 0; curr_row< num_rows; curr_row++)
   {
      for( int curr_col = 0; curr_col < num_cols; curr_col++)
      {
         suppress_mat.at<double>( curr_row, curr_col ) = 0;
      }
   }
   
   for( unsigned int curr_vec_feat = 0; curr_vec_feat < src_vec.size(); curr_vec_feat++ )
   {
      curr_feat = src_vec[ curr_vec_feat ]; 
      suppress_mat.at<double>( (int) curr_feat.pt.y, (int) curr_feat.pt.x ) = (double) curr_feat.response; 
   }
   
   for(unsigned int curr_vec_feat = 0; curr_vec_feat < src_vec.size(); curr_vec_feat++)
   {
      curr_feat = src_vec[ curr_vec_feat ];
      
      top_range = size_neighbor;
      bottom_range = size_neighbor;
      
      if( curr_feat.pt.y < size_neighbor )
      {
         top_range = curr_feat.pt.y;
      }
      if( curr_feat.pt.y >= num_rows - size_neighbor )
      {
         bottom_range = num_rows - curr_feat.pt.y;
      }
      
      right_range = size_neighbor;
      left_range = size_neighbor;
      
      if(curr_feat.pt.x < size_neighbor )
      {
         left_range = curr_feat.pt.x;
      }
      if( curr_feat.pt.x >= num_cols - size_neighbor )
      {
         right_range = num_cols - curr_feat.pt.x;
      }
         
      //Get the current pixel to check and set it to 90% of its
      //value to make sure that it is at least 10% greater in value
      //than its neighbors
      curr_check = curr_feat.response * 0.9;
      
      //Skip over points that are 0
      if( curr_check > 0 )
      {
         break_out = false;
         
         //Check the neigbhorhood to both the right and the bottom of the
         //current pixel. No need to check left and up because this is taken
         //care of by the right/bottom checking
         for(int curr_row = curr_feat.pt.y - top_range; curr_row < curr_feat.pt.y + bottom_range; curr_row++)
         {
            for( int curr_col = curr_feat.pt.x  - left_range; curr_col < curr_feat.pt.x + right_range; curr_col++)
            {
               //If neighboring pixel is less than current, then zero it
               //Else set the current pixel to be zero and move on
               
               if( curr_row == (int) curr_feat.pt.y && curr_col == (int) curr_feat.pt.x )
               {
                  break;
               }
               else if( curr_check > suppress_mat.at<double>( curr_row, curr_col ) )
               {
                  suppress_mat.at<double>( curr_row, curr_col ) = 0;
               }
               else
               {
                  suppress_mat.at<double>( (int) curr_feat.pt.y, (int) curr_feat.pt.x ) = 0;
                  break_out = true;
                  break;
               }
            }
            
            //Handles exiting the loops if current pixel was zeroed
            if( break_out )
            {
               break;
            }
         }
         
         if(!break_out)
         {
            dst_vec.push_back( curr_feat );
         }
      }
      
   }
   
   
   //Make sure we have features
   if( !dst_vec.empty() )
   {
      //Sort based on harris response value
      //sort( dst_vec.begin(), dst_vec.end(), auto_stitch::sort_response_val );
      
      //If more features than desired, get rid of the extra features.
      //if( dst_vec.size() >  num_keep )
      //{
    //     dst_vec.erase( dst_vec.begin() + num_keep, dst_vec.end() );
      //}
   }
}

