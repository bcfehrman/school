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


////////////////////
// Author: Brian Fehrman
// Computes the del values for the
// current backprop stage
////////////////////////
void ANN::compute_del()
{
   for( int del_idx = 0; del_idx < MAXH + 1; del_idx++ )
   {
      del[ del_idx ] = 0.0;
      
      for( int o_idx = 0; o_idx < MAXOUT; o_idx++ )
      {
         del[ del_idx ] += layers[ OUTPUT ][ o_idx ].error * 
                           layers[ OUTPUT ][ o_idx ].out_prime *
                           weights[ H_TO_O ][ del_idx ][ o_idx ].curr_w; 
      }
   } 
}

/////////////////////
// Author: Brian Fehrman
// Computes the output error
// for the current output
/////////////////////
DEC_TYPE ANN::compute_error( int f_des )
{
   DEC_TYPE curr_error = 0.0;

   for( int o_idx = 0; o_idx < MAXOUT; o_idx++ )
   {
      //Want index num correspondning to desired class number
      //to be 1 and all others to be 0
      if( o_idx == f_des )
      {
         layers[ OUTPUT ][ o_idx ].error = 1 - layers[ OUTPUT ][ o_idx ].out;
      }
      else
      {
         layers[ OUTPUT ][ o_idx ].error = 0 - layers[ OUTPUT ][ o_idx ].out;
      }
      
      curr_error += layers[ OUTPUT ][ o_idx ].error;
   }
   
   return curr_error;
}

//////////////////
// Author: Brian Fehrman
// Sigmoid activation function
// Can easily be changed to other functions
// but need to remember to change the derivative
// function too if training.
///////////////////
DEC_TYPE ANN::compute_f( const DEC_TYPE in_val )
{
   
   if( in_val > 10 )
   {
      return 1.0;
   }
   else if( in_val < -10 )
   {
      return 0.0;
   }
   
   return ( 1.0 / ( 1 + exp( -in_val ) ) );
}

///////////////////////
// Author: Brian Fehrman
// Computes the derivative of the sigmoid function
////////////////////////
DEC_TYPE ANN::compute_f_deriv( const DEC_TYPE in_val )
{
   return ( in_val * ( 1 - in_val ) );
}

////////////////////
// Author: Brian Fehrman
// Computes the feed forward between two layers
//////////////////////
void ANN::compute_forward( const perceptron *src, perceptron *dst, const weight **src_to_dst_weights, const int src_size, const int dst_size )
{
   for( int dst_idx = 0; dst_idx < dst_size; dst_idx++ )
   {
      dst[ dst_idx ].in = 0.0;
   
      for( int src_idx = 0; src_idx < src_size ; src_idx++ )
      {
         dst[ dst_idx ].in += src[ src_idx ].out * src_to_dst_weights[ dst_idx ][ src_idx ].curr_w;
      }
      
      dst[ dst_idx ].out = compute_f( dst[ dst_idx ].in );
      dst[ dst_idx ].out_prime = compute_f_deriv( dst[ dst_idx ].out );
   }
}

/////////////////
// Author: Brian Fehrman
// Initializes the class members for ANN
////////////////
void ANN::initialize()
{
   max_epochs = 300;
   tol = 0.01;
}

void ANN::input_layer_init( const DEC_TYPE *f_vals )
{
   for( int f_val_idx = 0; f_val_idx < MAXIN + 1; f_val_idx++ )
   {
      layers[ INPUT ][ f_val_idx ].out = f_vals[ f_val_idx ];
   }
}

void ANN::normalize_feat( feat_vec& curr_feat )
{
   cout << "MAX " << curr_feat.max_val << endl;
   
   for( unsigned int f_val_idx = 0; f_val_idx < MAXIN + 1; f_val_idx++ )
   {
      curr_feat.f_vals[ f_val_idx ] /= curr_feat.max_val;
   }
}

/////////////////////////////
// Author: Brian Fehrman
// Reads in the weights for each layer from the file
// specified by WEIGHT_FILE_NAME which is defined in constant.h
/////////////////////////////

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
      read_feature_set( fin, TRAIN );
   }
   
   read_feature_set( fin, TEST );   
   
   fin.close();
   
   return 0;
}

/////////////////////////////
// Author: Brian Fehrman
// Reads in the weights for each layer from the file
// specified by FV_IN which is defined in constant.h
////////////////////////////

int ANN::read_all_weights( const string w_file_name )
{
   //Open weight file using the given function parameter
   ifstream fin( w_file_name.c_str() );
   
   if( !fin )
   {
      cout << "Error opening file for reading: " << w_file_name << endl;
      return -1;
   }
   
   //Read in weights for input to hidden
   read_weight_set( fin, I_TO_H, MAXIN + 1, MAXH );
   
   //Read in weights for hidden to output
   read_weight_set( fin, H_TO_O, MAXH + 1, MAXOUT );
   
   fin.close();
   
   return 0;
}

/////////////////////
// Author: Brian Fehrman
//
///////////////////////
void ANN::read_feature_set( ifstream& fin, const int f_set)
{
   DEC_TYPE curr_val = 0.0;
   
   //Read in the training features
   for( int f_vec_idx = 0; f_vec_idx < NUMUV; f_vec_idx++ )
   {
      //Read in class of current feature
      fin >> input_vecs[ f_set ][ f_vec_idx ].class_num;
      cout << input_vecs[f_set][f_vec_idx].class_num << endl;
      //If FONT_DATA flag is set, read in the font type
      //and increment the starting feat_val index
      if( FONT_DATA )
      {
         fin >> input_vecs[ f_set ][ f_vec_idx ].font_type;
      }
      
      //Empty initalize parameter so that it starts
      //from where it was set in the above few lines
      for( int f_val_idx = 0; f_val_idx < MAXIN ; f_val_idx++ )
      {
         fin >> curr_val;
         input_vecs[ f_set][ f_vec_idx ].f_vals[ f_val_idx ] = curr_val;
         
         //Keep track of maximum value for current feature in case
         //we need to normalize
         if( curr_val > input_vecs[ f_set ][ f_vec_idx ].max_val )
         {
            input_vecs[ f_set ][ f_vec_idx ].max_val = curr_val;
         }
      }
      
      //bias input
      input_vecs[ f_set ][ f_vec_idx ].f_vals[ MAXIN ] = -1;
      
      //Normalize current feature vector is flag is set
      if( NORMALIZE )
      {
         //normalize_feat( input_vecs[ feat_set ][ feat_idx ] );
      }
   }
}

/////////////////////////
// Author: Brian Fehrman
// Reads in a set of weights
////////////////////////
void ANN::read_weight_set( ifstream& fin, const int w_set, const int src_size, const int dst_size )
{
   for( int dst_idx = 0; dst_idx < dst_size; dst_idx++ )
   {
      for( int src_idx = 0; src_idx < src_size; src_idx++ )
      {
         fin >> weights[ w_set ][ dst_idx ][ src_idx ].curr_w;
      }
   }
}

/////////////////
// Author: Brian Fehrman
// Run method for the ANN class. Will call
// methods as needed to run the ANN.
////////////////////////
int ANN::run( const bool train_first )
{
   this->train_first = train_first;
   
   initialize(); 
   
   read_all_features();
   
   if( train_first )
   {
      train();
   }
   
   return 0;
}

void ANN::train()
{
   int curr_epoch = 0;
   //read in weights for each layer pair
   read_all_weights( WEIGHT_FILE_NAME );
   
   curr_epoch_error = 100000;
   
   while( abs( curr_epoch_error ) > tol && curr_epoch < max_epochs )
   {
      curr_epoch_error = 0;
      
      for( int f_vec_idx = 0; f_vec_idx < NUMUV; f_vec_idx++ )
      {
         //Setup the feature values to be the output of the input
         //layer. Allows for reuse of code for the compute
         //forward function
         input_layer_init( input_vecs[ TRAIN ][ f_vec_idx ].f_vals );
         
         //Compute feed forwad from input to hidden layer
         compute_forward( (const perceptron*) layers[ INPUT ], (perceptron*) layers[ HIDDEN ], (const weight**) weights[ I_TO_H ], MAXIN + 1, MAXH );
         
         //Compute feed forward from hidden to output layer
         compute_forward( (const perceptron*) layers[ HIDDEN ], (perceptron*) layers[ OUTPUT ], (const weight**) weights[ H_TO_O ], MAXH + 1, MAXOUT );
         
         //Determine the current error and add it to the current epoch error;
         curr_epoch_error += compute_error( input_vecs[ TRAIN ][ f_vec_idx ].class_num );
         
         //Compute the del values
         compute_del();
         
         update_h_to_o_weights();
         update_i_to_h_weights();
       //  cout << input_vecs[ TRAIN ][ f_vec_idx ].class_num << endl;
         
         
         cout << "CURR OUT" << endl;
         
         for( int i = 0; i < 15; i++ )
         {
            cout << layers[ OUTPUT ][ i ].out << " ";
         }
         
         cout << endl << endl;
         
         cout << "DESIRED" << endl;
         
         for( int i = 0; i < 26; i++ )
         {
            if( i == input_vecs[ TRAIN ][ f_vec_idx ].class_num )
            {
               cout << 1 << " ";
            }
            else
            {
               cout << 0 << " ";
            }
         }
         
         cout << endl << endl;
         
         cout << "f_vec " << f_vec_idx << endl;
         
         
      }
      //cout << abs( curr_epoch_error ) << endl;        
      curr_epoch++;
      //cout << curr_epoch << endl;
   }
}

///////////////////////
// Author: Brian Fehrman
// Updates weights from hidden
// to output. Uses momentum.
////////////////////////
void ANN::update_h_to_o_weights()
{
   for( int h_idx = 0; h_idx < MAXH + 1; h_idx++ )
   {
      for( int o_idx = 0; o_idx < MAXOUT; o_idx++ )
      {
         weights[ H_TO_O ][ h_idx ][ o_idx ].prev_w = weights[ H_TO_O ][ h_idx ][ o_idx ].curr_w;
         
         weights[ H_TO_O ][ h_idx ][ o_idx ].curr_w = weights[ H_TO_O ][ h_idx ][ o_idx ].prev_w +
                                                      layers[ OUTPUT ][ o_idx ].error *
                                                      layers[ OUTPUT ][ o_idx ].out_prime *
                                                      layers[ HIDDEN ][ h_idx ].out * 
                                                      ETA + ALPHA * weights[ H_TO_O ][ h_idx ][ o_idx ].prev_change;
         
         //Update change in weights which is used for momentum
         weights[ H_TO_O ][ h_idx ][ o_idx ].prev_change = weights[ H_TO_O ][ h_idx ][ o_idx ].curr_w - 
                                                            weights[ H_TO_O ][ h_idx ][ o_idx ].prev_w;
                                                      
      }
   }
}

///////////////////////
// Author: Brian Fehrman
// Updates weights from input
// to hidden. Uses momentum.
////////////////////////
void ANN::update_i_to_h_weights()
{
   for( int in_idx = 0; in_idx < MAXIN + 1; in_idx++ )
   {
      for( int h_idx = 0; h_idx < MAXH; h_idx++ )
      {
         weights[ I_TO_H ][ in_idx ][ h_idx ].prev_w = weights[ I_TO_H ][ in_idx ][ h_idx ].curr_w;
         
         weights[ I_TO_H ][ in_idx ][ h_idx ].curr_w = weights[ I_TO_H ][ in_idx ][ h_idx ].prev_w +
                                                      layers[ INPUT ][ in_idx ].out *
                                                      del[ h_idx ] * ETA +  
                                                      ALPHA * weights[ I_TO_H ][ in_idx ][ h_idx ].prev_change;
         
         //Update change in weights which is used for momentum
         weights[ I_TO_H ][ in_idx ][ h_idx ].prev_change = weights[ I_TO_H ][ in_idx ][ h_idx ].curr_w - 
                                                            weights[ I_TO_H ][ in_idx ][ h_idx ].prev_w;
                                                      
      }
   }
}
