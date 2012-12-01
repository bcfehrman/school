/*
 * ann.h
 * 
 * Author: Brian Fehrman
 * Class: Pattern Recognition Fall 2012
 * 
 */


#ifndef ANN_H
#define ANN_H

/// Includes ///

#include <algorithm>
#include <cmath>
#include "constant.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/time.h>
#include <vector>

using namespace std;

/// Defines ///

//Use a define for decimal data type so that I can
//easily swap between floats, doubles, etc to see if
//it makes a difference
#define DEC_TYPE float

//Defines for indexing the layers
#define HIDDEN 1
#define INPUT 0
#define OUTPUT 2

//Defines for indexing the weights
#define H_TO_O 1
#define I_TO_H 0

//Defines for indexing the feature vectors
#define TEST 1
#define TRAIN 0


/// Structs ///

/////////////////
//Author: Brian Fehrman
// class_data struct holds statistic info for each class
// class_num - number of the class
// num_right - number of vectors classified right for the class
// num_wrong - number of vectors classified wrong for the class
////////////////
struct class_data
{
   int class_num;
   int num_right;
   int num_wrong;
   
   //Default constructor for the class_data struct
   class_data()
   {
      class_num = -1;
      num_right = 0;
      num_wrong = 0;
   }
};

/////////////////
//Author: Brian Fehrman
// feat_vec struct holds information for each feature vector
//
// class_num - number which identies the current class vector
// f_vals - the feature values for the current class vector
// font_type - font type of the current class vector
/////////////////
struct feat_vec
{
   int class_num;
   DEC_TYPE f_vals[ MAXIN + 1 ];
   int font_type;

   //Default constructor for the feature_vec struct
   feat_vec()
   {
      class_num = -1;
      font_type = -1;
   }
};

/////////////////
//Author: Brian Fehrman
// perceptron struct
//
// in - result of sum of weighted inputs
// out - result of input being fed through activation function
// out_prime - out fed through derivative of activation function
// error - difference between desired and actual output, only applicable
//          to the output nodes at the moment but I find it convenient
//          to tack it on to the peceptron struct.
/////////////////
struct perceptron
{
   DEC_TYPE in;
   DEC_TYPE out;
   DEC_TYPE out_prime;
   DEC_TYPE error;
   
   //Default contstructor for perceptron struct
   perceptron()
   {
      in = 0.0;
      out = 0.0;
      out_prime = 0.0;
      error = 100000.0;
   }
};

/////////////////
//Author: Brian Fehrman
// weight struct holds information for each weight
// 
// curr_w - the current weight value
// prev_change - the last change in weight wand is used for momentum
// prev_w - the previous weight value, used to calculate prev_change
/////////////////
struct weight
{
   DEC_TYPE curr_w;
   DEC_TYPE prev_change;
   DEC_TYPE prev_w;
   
   //Default constructor for weight struct
   weight()
   {
      curr_w = 1.0;
      prev_change = 0.0;
      prev_w = 1.0;
   }
};

class ANN
{
   public:
   
   //Public members
   
   //Public methods
      ANN();
      virtual ~ANN();
      int run( );
   
   private:
   
   //Private members
      class_data class_info[ MAXOUT ];
      DEC_TYPE curr_epoch_error;
      DEC_TYPE del[ MAXH + 1 ];
      feat_vec input_vecs[ 2 ][ NUMUV ];
      perceptron layers[ 3 ][ MAXOUT + 1 ];
      int max_epochs;
      int num_f_read;
      DEC_TYPE tol;
      bool train_first;
      weight weights[ 2 ][ MAXOUT + 1 ][ MAXOUT + 1 ];
      ofstream w_file_out;
      ifstream f_file_in, w_file_in;
 
   
   //Private methods
      void compute_del();
      DEC_TYPE compute_f( const DEC_TYPE in_val );
      DEC_TYPE compute_f_deriv( const DEC_TYPE in_val );
      DEC_TYPE compute_error( const int f_des );
      void compute_forward( const perceptron src[], perceptron dst[], const weight src_to_dst_weights[][MAXOUT + 1],
                              const int src_size, const int dst_size ); 
      void create_layers();
      int initialize();
      void input_layer_init( const DEC_TYPE f_vals[] );
      int find_output_class();
      void normalize_feat( feat_vec& curr_feat );
      void read_feature_set( const int feat_set );
      void read_weight_set( const int w_set, const int src_size, const int dst_size );
      void test();
      void train();
      void update_h_to_o_weights();
      void update_i_to_h_weights();
      int write_weights( const int w_set, const int src_size, const int dst_src );
};

#endif /* ANN_H */ 
