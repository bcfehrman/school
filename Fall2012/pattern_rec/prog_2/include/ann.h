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

#include "constant.h"
#include <fstream>
#include <iostream>
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

struct feat_vec
{
   int class_num;
   vector< DEC_TYPE > f_vals;
   int font_type;
   DEC_TYPE max_val;
   DEC_TYPE min_val;
   
   //Default constructor for the feature_vec struct
   feat_vec()
   {
      class_num = -1;
      font_type = -1;
      max_val = -100000.0;
      min_val = 100000.0;
   }
};

struct perceptron
{
   DEC_TYPE curr_in;
   DEC_TYPE curr_out;
   DEC_TYPE error;
   
   //Default contstructor for perceptron struct
   perceptron()
   {
      curr_in = 0;
      curr_out = 0;
      error = 100000;
   }
};

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
      int run();
   
   private:
   
   //Private members
   vector< class_data > class_info;
   DEC_TYPE curr_epoch_error;
   vector< DEC_TYPE > del;
   vector< vector< perceptron > > layers;
   vector< vector< feat_vec > > input_vecs;
   int max_epochs;
   DEC_TYPE tol;
   vector< vector< vector< weight > > > weights;
   
   //Private methods
   DEC_TYPE compute_f( DEC_TYPE in_val );
   DEC_TYPE compute_f_deriv( DEC_TYPE in_val );
   void compute_back();
   void compute_forward( vector< perceptron >& src, vector< perceptron >& dst, vector< vector < weight > >& src_to_dst_weights ); 
   void create_layers();
   void initialize();
   void normalize_feat( feat_vec& curr_feat );
   int read_all_features();
   void read_set_features( ifstream& fin, const int feat_set, const int num_feats, const int num_f_vals );
   void read_all_weights();
   void read_set_weights( ifstream& fin, const int w_set );
   void test();
   void train();
   void update_h_to_o_weights();
   void update_i_to_h_weights();
};

#endif /* ANN_H */ 
