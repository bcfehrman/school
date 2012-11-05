#include "libfeature_match.h"


/*********** Constructors **********/
Feautre_Match::Feature_Match()
{
   deriv_standard_deviation = 1.5;
   smooth_standard_deviation = 1.4;
   num_keep = 200;
   radius_suppress = 25;
   kernel_size = 5;
   num_scales = 5;
   feature_size = 13;
   feature_size_sqd = feature_size * feature_size;
   feature_size_div_2 = feature_size / 2;
   
   srand((unsigned)time(0));
   
}

Feature_Match::Feature_Match( float deriv_standard_deviation, float smooth_standard_deviation, int num_keep, int radius_suppress, int kernel_size, int num_scales, int feature_size)
{
   this->deriv_standard_deviation = deriv_standard_deviation;
   this->smooth_standard_deviation = smooth_standard_deviation;
   this->num_keep = num_keep;
   this->radius_suppress = radius_suppress;
   this->kernel_size = kernel_size;
   this->num_scales = num_scales;
   this->feature_size = feature_size;
   feature_size_sqd = feature_size * feature_size;
   feature_size_div_2 = feature_size / 2;
   
   srand((unsigned)time(0));
}

/************** Public Functions *******************/

 void Feature_Match::create_all_kernels()
 {
   for(int i = 0; i < num_scales; i++)
   {
      norm_LOG_kernels.push_back(Mat.create( FEATURE_SIZE, FEATURE_SIZE, CV_32F ));
      create_norm_LOG_kernel( norm_LOG_kernels[ i ],  smooth_standard_deviation + ( i *  smooth_standard_deviation ));
      
      smooth_gauss.push_back(Mat.create( kernel_size + 2, kernel_size + 2, CV_32F ) );
      create_gaussian_kernel( smooth_gauss[ i ], smooth_standard_deviation + ( i *  smooth_standard_deviation ) );
      
      gauss_Gx_kernels.push_back(Mat.create( kernel_size, kernel_size, CV_32F ));
      gauss_Gy_kernels.push_back(Mat.create( kernel_size, kernel_size, CV_32F ));
      create_deriv_gaussian_kernels(  gauss_Gx_kernels[ i ], gauss_Gy_kernels[ i ], deriv_standard_deviation);
      
      colors[i] = Scalar(rand() % 255, rand() % 255, rand() % 255 );
   } 
 }
