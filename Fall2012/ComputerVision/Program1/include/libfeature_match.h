#ifndef _FEATURE_MATCH_
#define _FEATURE_MATCH_

#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

using namespace cv;
using namespace std;

#define NUM_SCALES 5
#define FEATURE_SIZE 13
#define FEATURE_SIZE_SQD 169
#define FEATURE_SIZE_DIV_2 6

struct feat_val{
  int i_pos;
  int j_pos;
  float intensity_val;
  float major_orientation_x;
  float major_orientation_y;
  float orientation_angle;
  Mat feat_Ix;
  Mat feat_Iy;
  Mat feature;
  Mat region_patch;
  float scale;
  int scale_num;
  vector<Point> feat_box_points;
};

struct matches{
   int i_pos_1;
   int j_pos_1;
   int i_pos_2;
   int j_pos_2;
   double difference;
   bool filled;
   
   matches(): filled(false), difference(-1.0) {} 
};

class Feature_Match
{
 
 public:
 
   Mat auto_corr_mat_1, gray_image_1, image_1_highlighted, orig_image_1;
   Mat auto_corr_mat_2, gray_image_2, image_2_highlighted, orig_image_2;
	clock_t begin, end;
   vector<CvScalar> colors;
   Mat combined_images;
   float deriv_standard_deviation;
   int feature_size;
   int feature_size_div_2;
   int feature_size_sqd;
   vector<feat_val> feat_vec_1, feat_vec_2;
   vector<Mat> gauss_Gx_kernels, gauss_Gy_kernels;
   Mat Gx, Gy, Ix, Iy, IxIy;
   vector<Mat> image_1_pyramid, image_2_pyramid;
   int kernel_size;
   vector<matches> match_vec;
   vector<Mat> norm_LOG_kernels;
   int num_keep;
   int num_scales;
   int radius_suppress;
   vector<Mat> smooth_gauss;
   const float smooth_standard_deviation;
 
 /********PUBLIC FUNCTIONS*************/ 
   Feature_Match();
   Feature_Match( float deriv_standard_deviation, float smooth_standard_deviation, int num_keep, int radius_suppress, int kernel_size, int num_scales, int feature_size); 
   void convolve_matrix_region( Mat& src_mat, const int start_row, const int end_row, const int start_col, const int end_col, Mat& dst_mat, Mat& kernel );
   void create_auto_corr_matrix( Mat& src_mat, Mat& dst_mat, Mat& IxIy, Mat& Ix_sqd, Mat& Iy_sqd, const float threshold_val);
   void create_feat_boxes( vector<feat_val>& feat_vec );
   void create_derive_kernels( Mat& Gx, Mat& Gy);
   void create_deriv_gaussian_kernels( Mat& kern_X_dst, Mat& kern_y_dst, float standard_deviation);
   void create_gaussian_kernel( Mat& kern_dst, float standard_deviation);
   void create_all_kernels();
   void create_norm_LOG_kernel( Mat& kern_dst, float standard_deviation);
   void draw_boxes( Mat& src, vector<Point> feat_box_points, CvScalar color, int thickness );
   void extract_features( vector<Mat>& src_mat, vector<feat_val>& feat_vec, vector<Mat> smooth_gauss, const float start_STD, Mat& src_x_kern, Mat src_y_kern  );
   void find_orientations( vector<feat_val>& feat_vec, Mat& src_x_kern, Mat src_y_kern );
   void find_matches(const int rows, const int cols, vector<feat_val>& feat_vec_1, vector<feat_val>& feat_vec_2, vector<matches>& match_vec, const float threshold_val );
   void find_scales( vector<Mat>& src_mat, vector<feat_val>& feat_vec, vector<Mat> LOG_kernels, const float start_variance );
   float scaler_convolve_matrix_region( Mat& src_mat, Mat& kernel, const int i_pos, const int j_pos);
   bool sort_feat_val( feat_val feat_val_1, feat_val feat_val_2 );
   void suppress_non_maximums_adaptive( Mat& src_mat, vector<feat_val>& feat_vec, const int size_neighbor, const unsigned int num_keep);
}

#endif
