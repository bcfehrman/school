#ifndef _VISIONFUNCTIONS_
#define _VISIONFUNCTIONS_

#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace cv;
using namespace std;

#define NUM_SCALES 5
#define FEATURE_SIZE 10
#define FEATURE_SIZE_SQD 100
#define FEATURE_SIZE_DIV_2 5

struct feat_val{
  int i_pos;
  int j_pos;
  float intensity_val;
  float major_orientation_x;
  float major_orientation_y;
  Mat feat_Ix;
  Mat feat_Iy;
  Mat feature;
  float scale; 
};

struct matches{
   int i_pos_1;
   int j_pos_1;
   int i_pos_2;
   int j_pos_2;
};

void convolve_matrix_region( Mat& src_mat, const int start_row, const int end_row, const int start_col, const int end_col, Mat& dst_mat, Mat& kernel );
void create_auto_corr_matrix( Mat& src_mat, Mat& dst_mat, Mat& IxIy, Mat& Ix_sqd, Mat& Iy_sqd, const float threshold_val);
void create_derive_kernels( Mat& Gx, Mat& Gy);
void create_deriv_gaussian_kernels( Mat& kern_X_dst, Mat& kern_y_dst, float standard_deviation);
void create_gaussian_kernel( Mat& kern_dst, float standard_deviation);
void create_norm_LOG_kernel( Mat& kern_dst, float standard_deviation);
void extract_features( Mat& src_mat, vector<feat_val>& feat_vec, Mat smooth_gauss[ NUM_SCALES ], const float start_STD );
void find_orientations( Mat& src_mat, vector<feat_val>& feat_vec, Mat& src_x_kern, Mat src_y_kern );
void find_matches( vector<feat_val>& feat_vec1, vector<feat_val>& feat_vec2, vector<matches>& match_vec, const float threshold_val );
void find_scales( Mat& src_mat, vector<feat_val>& feat_vec, Mat LOG_kernels[NUM_SCALES], float start_variance );
float scaler_convolve_matrix_region( Mat& src_mat, Mat& kernel, const int i_pos, const int j_pos);
bool sort_feat_val( feat_val feat_val_1, feat_val feat_val_2 );
void suppress_non_maximums_adaptive( Mat& src_mat, vector<feat_val>& feat_vec, const int size_neighbor, const unsigned int num_keep);

#endif
