#include "libvision_functions.h"

void convolve_matrix_region( Mat& src_mat, const int start_row, const int end_row, const int start_col, const int end_col, Mat& dst_mat, Mat& kernel )
{   
   int size_kernel = kernel.rows;
   int size_kernel_div_2 = size_kernel / 2;
   int adjusted_start_row = start_row;
   int adjusted_end_row = end_row;
   int adjusted_start_col = start_col;
   int adjusted_end_col = end_col;
   
   if( adjusted_start_row < 0)
      adjusted_start_row = 0;
      
   if( adjusted_start_col < 0)
      adjusted_start_col = 0;
      
   if( adjusted_end_row > src_mat.rows)
      adjusted_end_row = src_mat.rows;
      
   if( adjusted_end_col > src_mat.cols)
      adjusted_end_col = src_mat.cols;   
   
   Mat sub_mat = src_mat.colRange(adjusted_start_col, adjusted_end_col).rowRange(adjusted_start_row, adjusted_end_row);
   dst_mat = sub_mat;
   //filter2D( sub_mat, sub_mat, -1, kernel );
   //dst_mat = sub_mat;
   //if( size_kernel_div_2 + ( end_col - start_col) 
   
   //dst_mat = sub_mat.colRange( size_kernel_div_2, size_kernel_div_2 + ( end_col - start_col) ).rowRange( size_kernel_div_2, size_kernel_div_2 + ( end_row - start_row) );
}

//Generates a matrix of the autocorrelation value found at each point.
//Algorithm taken from Szeliski page 188.
void create_auto_corr_matrix( Mat& src_mat, Mat& dst_mat, Mat& IxIy, Mat& Ix_sqd, Mat& Iy_sqd, const float threshold_val)
{
   int num_rows = src_mat.rows;
   int num_cols = src_mat.cols;
   float curr_Ix_sqd, curr_Iy_sqd, curr_IxIy;
   float trace_val, det_val, intensity_val;
   
   for( int i = 0; i < num_rows; i++ )
   {
      for( int j = 0; j < num_cols; j++ )
      {
         //So we don't have to unnecessarily index into the matrix
         curr_Ix_sqd = Ix_sqd.at<float>( i, j );
         curr_Iy_sqd = Iy_sqd.at<float>( i, j );
         curr_IxIy = IxIy.at<float>( i, j ); 
         
         //Calculate trace and determinant of the 2x2 matrix
         det_val = ( curr_Ix_sqd * curr_Iy_sqd ) - ( curr_IxIy * curr_IxIy );
         trace_val = ( curr_Ix_sqd  + curr_Iy_sqd);
         intensity_val = det_val / trace_val;
         
         //Suppress values less than threshold
         if( intensity_val > threshold_val )
         {
            dst_mat.at<float>( i, j ) = intensity_val;
         }
         else
         {
            dst_mat.at<float>( i, j ) = 0;
         }
      } 
   }
}

//Creates (hopefully) the rotated boxes around the features
void create_feat_boxes( vector<feat_val>& feat_vec )
{
   Point p1( -FEATURE_SIZE_DIV_2, FEATURE_SIZE_DIV_2);
   Point p2( FEATURE_SIZE_DIV_2, FEATURE_SIZE_DIV_2);
   Point p3( FEATURE_SIZE_DIV_2, -FEATURE_SIZE_DIV_2);
   Point p4( -FEATURE_SIZE_DIV_2, -FEATURE_SIZE_DIV_2);
   Point tempPoint;
   feat_val curr_feat;
   
   for( unsigned int i = 0; i < feat_vec.size(); i++)
   {
      curr_feat = feat_vec.at( i );
      
      tempPoint.x = ( p1.x * (curr_feat.scale_num) * cos( curr_feat.orientation_angle) - p1.y * (curr_feat.scale_num) * sin(curr_feat.orientation_angle) ) + curr_feat.j_pos;
      tempPoint.y = ( p1.x * (curr_feat.scale_num) * sin( curr_feat.orientation_angle) + p1.y * (curr_feat.scale_num) * cos(curr_feat.orientation_angle) ) + curr_feat.i_pos;
         
      feat_vec.at(i).feat_box_points.push_back( tempPoint );
      
      tempPoint.x = ( p2.x * (curr_feat.scale_num) * cos( curr_feat.orientation_angle) - p2.y * (curr_feat.scale_num) * sin(curr_feat.orientation_angle) ) + curr_feat.j_pos;
      tempPoint.y = ( p2.x * (curr_feat.scale_num) * sin( curr_feat.orientation_angle) + p2.y * (curr_feat.scale_num) * cos(curr_feat.orientation_angle) ) + curr_feat.i_pos;
         
      feat_vec.at(i).feat_box_points.push_back( tempPoint );
      
      tempPoint.x = ( p3.x * (curr_feat.scale_num) * cos( curr_feat.orientation_angle) - p3.y * (curr_feat.scale_num) * sin(curr_feat.orientation_angle) ) + curr_feat.j_pos;
      tempPoint.y = ( p3.x * (curr_feat.scale_num) * sin( curr_feat.orientation_angle) + p3.y * (curr_feat.scale_num) * cos(curr_feat.orientation_angle) ) + curr_feat.i_pos;
         
      feat_vec.at(i).feat_box_points.push_back( tempPoint );
      
      tempPoint.x = ( p4.x * (curr_feat.scale_num) * cos( curr_feat.orientation_angle) - p4.y * (curr_feat.scale_num) * sin(curr_feat.orientation_angle) ) + curr_feat.j_pos;
      tempPoint.y = ( p4.x * (curr_feat.scale_num) * sin( curr_feat.orientation_angle) + p4.y * (curr_feat.scale_num) * cos(curr_feat.orientation_angle) ) + curr_feat.i_pos;
         
      feat_vec.at(i).feat_box_points.push_back( tempPoint );
   }
}

//Creates 3x3 Sobel operator kernels that
//are used to approximate the x and y
//derivatives of a matrix
void create_derive_kernels( Mat& Gx, Mat& Gy)
{
   int val_1 = -1;
   int val_2 = -2;
   
   Gx.create(3, 3, CV_32F);
   Gx.create(3, 3, CV_32F);
   
   //This is just a slightly more compact
   //way to generate the matrices instead
   //of hardcoding each value
   //val_1 and val_2 seem non-descriptive
   //but they refer to the multiple of values that
   //appear (i.e., val_1 = -1, 0, 1)
   for(int i = 0; i < 3; i++)
   {
      Gx.at<float>(0, i) = val_1;
      Gx.at<float>(1, i) = val_2;
      Gx.at<float>(2, i) = val_1;
      
      Gy.at<float>(i, 0) = val_1;
      Gy.at<float>(i, 1) = val_2;
      Gy.at<float>(i, 2) = val_1;
      
      val_1 += 1;
      val_2 += 2;
   }
   
   //Flip around both axes since opencv uses correlation
   //instead of convolution although I'm still not convinced
   //that it matters for these matrices.
   flip(Gx, Gx, -1);
   flip(Gy, Gy, -1);
}

//Creates a gaussian kernel which will be the size of the
//destination matrix
void create_deriv_gaussian_kernels( Mat& kern_x_dst, Mat& kern_y_dst, float standard_deviation)
{
	int num_cols = kern_x_dst.cols;
   int num_rows = kern_x_dst.rows;
   int num_cols_div_2 = num_cols / 2;
   int num_rows_div_2 = num_rows / 2;
	float variance = standard_deviation * standard_deviation;
	float num = 0;
	float den = -sqrt(2 * M_PI);
	int x = 0, y = 0, y_sqrd = 0;
	float kernel_val = 0;
   
   float* kern_x_ptr_head = (float*) kern_x_dst.data;
   float* kern_x_ptr = kern_x_ptr_head;
   float* kern_y_ptr_head = (float*) kern_y_dst.data;
   float* kern_y_ptr = kern_y_ptr_head;

   //Generate the gaussian kernel
	for(int i = 0; i < num_rows; i++)
	{
		y = i - num_rows_div_2;
		y_sqrd = y * y;
		
      for(int j = 0; j < num_cols; j++)
      {
         x = j - num_cols_div_2;
      
         num =  exp(-1 * ( ( x * x + y_sqrd) / ( 2 * variance ) ) );

         kernel_val = num / den;

         if( x == 0 )
         {
            *kern_x_ptr = 0;
         }
         else
         {
            *kern_x_ptr = kernel_val * x;
         }
         
         kern_x_ptr++;
         
         if( y == 0 )
         {
            *kern_y_ptr = 0;
         }
         else
         {
            *kern_y_ptr = kernel_val * y;
         }
         kern_y_ptr++;
      }
   }
}

//Creates a gaussian kernel which will be the size of the
//destination matrix
void create_gaussian_kernel( Mat& kern_dst, float standard_deviation)
{
	int num_cols = kern_dst.cols;
   int num_rows = kern_dst.rows;
   int num_cols_div_2 = num_cols / 2;
   int num_rows_div_2 = num_rows / 2;
	float variance = standard_deviation * standard_deviation;
	float num = 0;
	float den = 2 * M_PI;
	int x = 0, y_sqrd = 0;
	float kernel_val = 0;
	float pre_norm_sum = 0;
	float norm_sum = 0;
   float* kern_ptr_head = (float*) kern_dst.data;
   float* kern_ptr = kern_ptr_head;

   //Generate the gaussian kernel
	for(int i = 0; i < num_rows; i++)
	{
		y_sqrd = i - num_rows_div_2;
		y_sqrd *= y_sqrd;
		
      for(int j = 0; j < num_cols; j++)
      {
         x = j - num_cols_div_2;
      
         num =  exp(-1 * ( ( x * x + y_sqrd) / ( 2 * variance ) ) );
         
         kernel_val = num / den;
         
         *kern_ptr = kernel_val;
         kern_ptr++;
         
         pre_norm_sum += kernel_val;
      }
   }
   
   //Reset the pointer
   kern_ptr = kern_ptr_head;
   
   //Normalize the values
   for(int i = 0; i < num_rows; i++)
   {
      for( int j = 0; j < num_cols; j++ )
      {
         *kern_ptr = *kern_ptr / pre_norm_sum;
         norm_sum += *kern_ptr;
   
         kern_ptr++;
      }
   }
}

//Creates a gaussian kernel which will be the size of the
//destination matrix
void create_norm_LOG_kernel( Mat& kern_dst, float standard_deviation)
{
	int num_cols = kern_dst.cols;
   int num_rows = kern_dst.rows;
   int num_cols_div_2 = num_cols / 2;
   int num_rows_div_2 = num_rows / 2;
	float variance = standard_deviation * standard_deviation;
	int xSqrd = 0, y_sqrd = 0;
	float kernel_val = -2 * variance;
	float pre_norm_sum = 0;
	float norm_sum = 0;
   float* kern_ptr_head = (float*) kern_dst.data;
   float* kern_ptr = kern_ptr_head;

   //Generate the scale normalized Laplacian of Gaussian kernel
	for(int i = 0; i < num_rows; i++)
	{
		y_sqrd = i - num_rows_div_2;
		y_sqrd *= y_sqrd;
      kernel_val += y_sqrd;
		
      for(int j = 0; j < num_cols; j++)
      {
         xSqrd = j - num_cols_div_2;
         xSqrd *= xSqrd;
         kernel_val += xSqrd;
         kernel_val *=  exp(-1 * ( ( xSqrd + y_sqrd) / ( 2 * variance ) ) );
         
         *kern_ptr = kernel_val;
         kern_ptr++;
         
         pre_norm_sum += kernel_val;
      }
   }
   
   //Reset the pointer
   kern_ptr = kern_ptr_head;
   
   //Normalize the values
   for(int i = 0; i < num_rows; i++)
   {
      for( int j = 0; j < num_cols; j++ )
      {
         *kern_ptr = *kern_ptr / pre_norm_sum;
         norm_sum += *kern_ptr;
   
         kern_ptr++;
      }
   }
}

void draw_boxes( Mat& src, vector<Point> feat_box_points, CvScalar color, int thickness )
{
   for(unsigned int i = 0; i < feat_box_points.size() - 1; i++ )
   {
      line( src, feat_box_points.at(i), feat_box_points.at(i + 1), color, thickness );
   }
   
   line( src, feat_box_points.at(feat_box_points.size() - 1), feat_box_points.at(0), color, thickness );
}

void extract_features( vector<Mat>& src_mat, vector<feat_val>& feat_vec, Mat smooth_gauss[ NUM_SCALES ], const float start_STD, Mat& src_x_kern, Mat src_y_kern  )
{
   feat_val curr_feature;
   int curr_scale = 0;
   float sqrt_2 = sqrt(2.0);
   float curr_pow;
   
   for( unsigned int i = 0; i < feat_vec.size(); i++ )
   {
      //Make temporay region large enough to accomodate rotation of feature patch
      feat_vec.at( i ).region_patch.create(FEATURE_SIZE * sqrt_2, FEATURE_SIZE * sqrt_2, CV_32F);
      curr_scale = int ( feat_vec.at( i ).scale / start_STD ) - 1;
      
      curr_pow = pow(2, curr_scale);
      
      //Extract feature region at dominant smoothing scale
      convolve_matrix_region( src_mat.at(curr_scale), feat_vec.at( i ).i_pos / curr_pow - ( FEATURE_SIZE_DIV_2 * sqrt_2 ),
         feat_vec.at( i ).i_pos / curr_pow + ( FEATURE_SIZE_DIV_2 * sqrt_2 + 1 ), feat_vec.at( i ).j_pos / curr_pow - ( FEATURE_SIZE_DIV_2 * sqrt_2 ), 
         feat_vec.at( i ).j_pos / curr_pow + ( FEATURE_SIZE_DIV_2 * sqrt_2 + 1 ), feat_vec.at( i ).region_patch, 
         smooth_gauss[ curr_scale ] );
   }
 
   find_orientations( feat_vec, src_x_kern, src_y_kern );
}

void find_matches(const int rows, const int cols, vector<feat_val>& feat_vec_1, vector<feat_val>& feat_vec_2,
                     vector<matches>& match_vec, const float threshold_val)
{
   float difference = 0.0;
   float min_difference = 0.0;
   float second_min_difference = -1;
   float temp = 0.0;
   matches curr_match;
   int curr_match_i, curr_match_j;
   int match_j = 0;
   vector<vector<matches> > potential_matches ( rows, vector<matches> ( cols ) );
   
   
   for( unsigned int i = 0; i < feat_vec_1.size(); i++ )
   {
      min_difference = 10000000000;
      second_min_difference = -1;
      
      for( unsigned int j = 0; j < feat_vec_2.size(); j++ )
      {
         difference = 0.0;
         
         for( int curr_row = 0; curr_row < FEATURE_SIZE; curr_row++ )
         {
            for( int curr_col = 0; curr_col < FEATURE_SIZE; curr_col++ )
            {
               temp = fabs(feat_vec_1.at( i ).feature.at<float>( curr_row, curr_col) - feat_vec_2.at( j ).feature.at<float>( curr_row, curr_col));
               difference += temp;
            }
         }
         
         if(difference < min_difference )
         {
            second_min_difference = min_difference;
            min_difference = difference;
            match_j = j;
         }
      }
      
      //if(min_difference < threshold_val)
      if( min_difference < threshold_val && abs(min_difference / second_min_difference) < 0.6)
      {
         curr_match_i = feat_vec_2.at( match_j ).i_pos;
         curr_match_j = feat_vec_2.at( match_j ).j_pos;
         
         curr_match.i_pos_1 = feat_vec_1.at( i ).i_pos;
         curr_match.j_pos_1 = feat_vec_1.at( i ).j_pos;
         curr_match.feat_1 = feat_vec_1.at( i ).feature;
         curr_match.feat_2 = feat_vec_2.at( match_j ).feature;
         
         curr_match.i_pos_2 = curr_match_i;
         curr_match.j_pos_2 = curr_match_j;
         
         curr_match.difference = min_difference;
         
         if( !potential_matches[curr_match_i][curr_match_j].filled )
         {
            potential_matches[curr_match_i][curr_match_j] = curr_match;
            potential_matches[curr_match_i][curr_match_j].filled = true;
         }
         else if( curr_match.difference < potential_matches[curr_match_i][curr_match_j].difference )
         {
            potential_matches[curr_match_i][curr_match_j] = curr_match;
         }
      }
   }
   
   for(int i = 0; i < rows; i++)
   {
      for(int j = 0; j < cols; j++)
      {
         if(potential_matches[i][j].filled)
         {
            match_vec.push_back(potential_matches[i][j]);
         }
      }
   }
}

void find_orientations( vector<feat_val>& feat_vec, Mat& src_x_kern, Mat src_y_kern )
{
   Mat feat_Ix, feat_Iy;
   float Ix_sum = 0.0;
   float Iy_sum = 0.0;
   float mag_Ix_Iy;
   float angle;
   float tmp_x, tmp_y;
   float new_x, new_y;
   int equiv_row, equiv_col;
   int size_diff = FEATURE_SIZE * sqrt( 2.0 ) - FEATURE_SIZE;
   
   for(unsigned int i = 0; i < feat_vec.size(); i++ )
   {
      feat_vec.at( i ).feature.create( FEATURE_SIZE, FEATURE_SIZE, CV_32F );
      
      filter2D( feat_vec.at( i ).region_patch, feat_Ix, -1, src_x_kern );
      filter2D( feat_vec.at( i ).region_patch, feat_Iy, -1, src_y_kern );
   
      Ix_sum = sum( feat_Ix )[ 0 ];
      Iy_sum = sum( feat_Iy )[ 0 ];

      //Average derivative for each directions
      feat_vec.at( i ).major_orientation_x = Ix_sum / FEATURE_SIZE_SQD;
      feat_vec.at( i ).major_orientation_y = Iy_sum / FEATURE_SIZE_SQD;

      //Normalize
      mag_Ix_Iy = sqrt( feat_vec.at( i ).major_orientation_x * feat_vec.at( i ).major_orientation_x + 
         feat_vec.at( i ).major_orientation_y * feat_vec.at( i ).major_orientation_y);
      
      feat_vec.at( i ).major_orientation_x /= mag_Ix_Iy;
      feat_vec.at( i ).major_orientation_y /= mag_Ix_Iy;
      
      angle = atan2( feat_vec.at( i ).major_orientation_y , feat_vec.at( i ).major_orientation_x );
      
      feat_vec.at( i ).orientation_angle = angle;
      
      for( int curr_row = 0; curr_row < FEATURE_SIZE; curr_row++ )
      {
         tmp_y = curr_row - FEATURE_SIZE_DIV_2;
         
         for( int curr_col = 0; curr_col < FEATURE_SIZE; curr_col++ )
         {
            tmp_x = curr_col -  FEATURE_SIZE_DIV_2;
            
            new_x = tmp_x * cos( angle ) - tmp_y * sin( angle );
            new_y = tmp_x * sin( angle ) + tmp_y * cos( angle );
            
            equiv_col = new_x + FEATURE_SIZE_DIV_2;// + size_diff;
            equiv_row = new_y + FEATURE_SIZE_DIV_2;// + size_diff;
            
            feat_vec.at(i).feature.at<float>( curr_row, curr_col ) = feat_vec.at(i).region_patch.at<float>( equiv_row, equiv_col);
         }
      }
      
      //Normalize 
      feat_vec.at( i ).feature /= sum( feat_vec.at( i ).feature)[ 0 ];
   }
}

//Uses the Laplacian of Gaussian to find the scale at which the current
//feature is an extrema and sets this as the feature's characteristic
//scale
void find_scales( vector<Mat>& src_mat, vector<feat_val>& feat_vec, Mat LOG_kernels[NUM_SCALES], const float start_STD)
{
   float max_response = 0.0;
   float curr_scale = start_STD;
   float max_scale = 0.0;
   float curr_response = 0.0;
   int curr_pow = 1;
   
   for( unsigned int i = 0; i < feat_vec.size(); i++ )
   {
      curr_scale = start_STD;
      max_response = -100000.0;
                 
      for(int j = 0; j < NUM_SCALES; j++ )
      {  
         curr_pow = pow( 2, j );     
         
         //Get the Laplacian of Gaussian measurement for the current feature point
         curr_response = scaler_convolve_matrix_region( src_mat.at(j), LOG_kernels[j], feat_vec.at(i).i_pos / curr_pow, feat_vec.at(i).j_pos / curr_pow );
         
         //Want either min or max
         curr_response *= curr_response;
         
         if( curr_response > max_response )
         {
            max_response = curr_response;
            max_scale = curr_scale;
         }

         curr_scale += start_STD;
      }
      
      feat_vec.at(i).scale = max_scale;
      feat_vec.at(i).scale_num = int(max_scale / start_STD);
   }
}

//Puts the center of the kernel on the matConvCenter point in src_mat and then
//performs convolution at the point and returns the result
float scaler_convolve_matrix_region( Mat& src_mat, Mat& kernel, const int i_pos, const int j_pos)
{
   float result = 0.0;
   int src_mat_cols = src_mat.cols;
   int src_mat_rows = src_mat.rows;
   int kernel_size = kernel.rows;
   int kernel_size_div_2 = kernel_size / 2;
   int curr_i_pos = i_pos - kernel_size_div_2;
   int curr_j_pos = 0;
   
   for( int i = 0; i < kernel_size; i++, curr_i_pos++ )
   {
      if( curr_i_pos < 0 )
         continue;
      else if( curr_i_pos >= src_mat_rows )
         break;
         
      curr_j_pos = j_pos - kernel_size_div_2;
      
      for( int j = 0; j < kernel_size; j++, curr_j_pos++ )
      {
               
         if( curr_j_pos < 0 )
            continue;
         else if( curr_j_pos >= src_mat_cols )
            break;

         result += ( kernel.at<float>( i, j ) * src_mat.at<float>( curr_i_pos, curr_j_pos ) ) ;
      }
   }
   
   return result;
}

//Sort function used for sorting a vector of feat_val structs
//and is based upon their intensity response value wrt the
//harrison operator.
bool sort_feat_val( feat_val feat_val_1, feat_val feat_val_2 )
{
   return ( feat_val_1.intensity_val < feat_val_2.intensity_val );
}

void suppress_non_maximums_adaptive( Mat& src_mat, vector<feat_val>& dst_vec, const int size_neighbor, const unsigned int num_keep)
{
   float curr_check = 0.0;
   bool break_out = false;
   int num_rows = src_mat.rows;
   int num_cols = src_mat.cols;
   int top_range = size_neighbor;
   int bottom_range = size_neighbor;
   int left_range = size_neighbor;
   int right_range = size_neighbor;
   feat_val curr_feat;
   
   for(int i = FEATURE_SIZE; i < num_rows - FEATURE_SIZE; i++)
   {
      if(i < size_neighbor )
      {
         top_range = i;
      }
      else if( i >= num_rows - size_neighbor )
      {
         bottom_range = num_rows - i;
      }
      
      right_range = size_neighbor;
      left_range = size_neighbor;
      
      for( int j = FEATURE_SIZE; j < num_cols - FEATURE_SIZE; j++ )
      {
         if(j < size_neighbor )
         {
            left_range = j;
         }
         else if( j >= num_cols - size_neighbor )
         {
            right_range = num_cols - j;
         }
         
         //Get the current pixel to check and set it to 90% of its
         //value to make sure that it is at least 10% greater in value
         //than its neighbors
         curr_check = src_mat.at<float>(i,j) * 0.9;
         
         //Skip over points that are 0
         if( curr_check != 0 )
         {
            break_out = false;
            
            //Check the neigbhorhood to both the right and the bottom of the
            //current pixel. No need to check left and up because this is taken
            //care of by the right/bottom checking
            for(int curr_row = i - top_range; curr_row < i + bottom_range; curr_row++)
            {
               for( int curr_col = j  - left_range; curr_col < j + right_range; curr_col++)
               {
                  //If neighboring pixel is less than current, then zero it
                  //Else set the current pixel to be zero and move on
                  
                  if( curr_row == i && curr_col == j )
                  {
                     break;
                  }
                  else if( curr_check >= src_mat.at<float>( curr_row, curr_col ) )
                  {
                     src_mat.at<float>( curr_row, curr_col ) = 0;
                  }
                  else
                  {
                     src_mat.at<float>( i, j ) = 0;
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
               curr_feat.i_pos = i;
               curr_feat.j_pos = j;
               curr_feat.intensity_val = src_mat.at<float>( i, j );
               dst_vec.push_back( curr_feat );
            }
         }
      }
   }
   
   //Make sure we have features
   if( !dst_vec.empty() )
   {
      //Sort based on harris response value
      sort( dst_vec.begin(), dst_vec.end(), sort_feat_val );
      
      //If more features than desired, get rid of the extra features.
      if( dst_vec.size() >  num_keep )
      {
         dst_vec.erase( dst_vec.begin() + num_keep, dst_vec.end() );
      }
   }
}
