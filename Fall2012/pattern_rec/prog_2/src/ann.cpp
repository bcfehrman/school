/*
 * ann.cpp
 * Author: Brian Fehrman
 * Class: Pattern Rec Fall 2012
 */


#include "ann.h"

/// ANN Class Constructors ///
ANN::ANN()
{
   
}


ANN::~ANN()
{
   
}

/// ANN Methods ///

//////////////////
// Author: Brian Fehrman
// Sigmoid activation function
// Can easily be changed to other functions
// but need to remember to change the derivative
// function too if training.
///////////////////
DEC_TYPE ANN::compute_f( DEC_TYPE in_val )
{
   return ( 1.0 / ( 1 + exp( -in_val ) ) );
}

///////////////////////
// Author: Brian Fehrman
// Computes the derivative of the sigmoid function
////////////////////////
DEC_TYPE ANN::compute_f_deriv( DEC_TYPE in_val )
{
   return ( in_val - in_val * in_val );
}

////////////////////
// Author: Brian Fehrman
// Computes the feed forward between two layers
//////////////////////
void ANN::compute_forward( vector< perceptron >& src, vector< perceptron >& dst, vector< vector < weight > >& src_to_dst_weights )
{
   int dst_size = src_to_dst_weights.size();
   int src_size = src_to_dst_weights[ 0 ].size();
   
   for( int dst_idx = 0; dst_idx < dst_size; dst_idx++ )
   {
      dst[ dst_idx ].in = 0.0;
      
      for( int src_idx = 0; src_idx < src_size; src_idx++ )
      {
         dst[ dst_idx ].in += src[ src_idx ].out * src_to_dst_weights[ dst_idx ][ src_idx ].curr_w;
      }
      
      dst[ dst_idx ].out = compute_f( dst[ dst_idx ].in );
      dst[ dst_idx ].out_prime = compute_f_deriv( dst[ dst_idx ].in );
   }
}

/////////////////
// Author: Brian Fehrman
// Initializes the class members for ANN
////////////////
void ANN::initialize()
{
   int num_vec_sets = 2;
   int num_layers = 3;
   
   //Dynamically create space for the input vectors
   input_vecs.resize( num_vec_sets );
   
   //Create space for training vectors
   input_vecs[ TRAIN ].resize( NUMUV );
   
   //Create space for testing vectors
   input_vecs[ TEST ].resize( NUMUV );

   //Dynamically create space for layers
   layers.resize( num_layers );
   
   //Create space for input layer
   layers[ INPUT ].resize( MAXIN );

   //Create space for hidden layer
   layers[ HIDDEN ].resize( MAXH );
   
   //Create space for output layer
   layers[ HIDDEN ].resize( MAXOUT );
   
   //Dynamically create space for weights
   weights.resize( num_layers - 1 );
   
   //Create space for input to hidden weights
   weights[ I_TO_H ].resize( MAXH );
   for( unsigned int h_idx = 0; h_idx < weights[ I_TO_H ].size(); h_idx++ )
   {
      weights[ I_TO_H ][ h_idx ].resize( MAXIN +  1 );
   }
   
   //Create space for hidden to output weights
   weights[ H_TO_O ].resize( MAXOUT );
   for( unsigned int o_idx = 0; o_idx < weights[ H_TO_O ].size(); o_idx++ )
   {
      weights[ H_TO_O ][ o_idx ].resize( MAXH + 1 );
   }
   
   //Create space for del vector
   del.resize( MAXOUT );
   
   //Create space for class info vector which tracks classification stats
   //for each class
   class_info.resize( MAXOUT );
   
      //Check if font value is also on the line
   if( FONT_DATA )
   {
      num_f_vals = MAXIN + 2;
   }
   else
   {
      num_f_vals = MAXIN + 1;
   }
}

void ANN::input_layer_init()
{
   
}

void ANN::normalize_feat( feat_vec& curr_feat )
{
   cout << "MAX " << curr_feat.max_val << endl;
   
   for( unsigned int f_val_idx = 0; f_val_idx < curr_feat.f_vals.size(); f_val_idx++ )
   {
      curr_feat.f_vals[ f_val_idx ] /= curr_feat.max_val;
   }
}

int ANN::read_all_features()
{
   //Open feature vector file using the defined constant
   ifstream fin( FV_IN );
   
   if( !fin )
   {
      cout << "Error opening file for reading: " << FV_IN << endl;
      return -1;
   }

   //Only read in training data if desired to do so
   if( train_first )
   {
      read_feature_set( fin, TRAIN, input_vecs[ TRAIN ].size() );
   }
   
   read_feature_set( fin, TEST, input_vecs[ TEST ].size() );
   
   fin.close();
   
   return 0;
}

/////////////////////
// Author: Brian Fehrman
//
///////////////////////
void ANN::read_feature_set( ifstream& fin, const int feat_set, const int num_feat_vecs )
{
   DEC_TYPE curr_val = 0.0;
   int feat_val_idx = 1;
   
   //Read in the training features
   for( int feat_idx = 0; feat_idx < num_feat_vecs; feat_idx++ )
   {
      feat_val_idx = 1;
      
      //Read in class of current feature
      fin >> input_vecs[ feat_set ][ feat_idx ].class_num;
      
      //If FONT_DATA flag is set, read in the font type
      //and increment the starting feat_val index
      if( FONT_DATA )
      {
         fin >> input_vecs[ feat_set ][ feat_idx ].font_type;
         feat_val_idx++;
      }
      
      //Empty initalize parameter so that it starts
      //from where it was set in the above few lines
      for( ; feat_val_idx < num_f_vals; feat_val_idx++ )
      {
         fin >> curr_val;
         input_vecs[ feat_set][ feat_idx ].f_vals.push_back( curr_val );
         
         //Keep track of maximum value for current feature in case
         //we need to normalize
         if( curr_val > input_vecs[ feat_set ][ feat_idx ].max_val )
         {
            input_vecs[ feat_set ][ feat_idx ].max_val = curr_val;
         }
      }
      
      //Normalize current feature vector is flag is set
      if( NORMALIZE )
      {
         //normalize_feat( input_vecs[ feat_set ][ feat_idx ] );
      }
   }
}

/////////////////
// Author: Brian Fehrman
// Run method for the ANN class. Will call
// methods as needed to run the ANN.
////////////////////////
int ANN::run( bool train_first )
{
   this->train_first = train_first;
   
   initialize(); 
   
   read_all_features();
   
   return 0;
}
