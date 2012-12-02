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
void ANN::compute_del( )
{
   int del_idx = 0;
   int out_idx = 0;
   
   //For each hidden node
   for( del_idx = 0; del_idx < MAXH + 1; del_idx++ )
   {
      del[ del_idx ] = 0.0;
      
      //For each output node
      for( out_idx = 0; out_idx < MAXOUT; out_idx++ )
      {
         del[ del_idx ] += layers[ OUTPUT ][ out_idx ].error * 
                           layers[ OUTPUT ][ out_idx ].out_prime *
                           weights[ H_TO_O ][ del_idx ][ out_idx ].curr_w; 
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
   int out_idx = 0;

   //for each output node
   for( out_idx = 0; out_idx < MAXOUT; out_idx++ )
   {
      //Want index num correspondning to desired class number
      //to be 1 and all others to be 0
      if( out_idx == f_des )
      {
         layers[ OUTPUT ][ out_idx ].error = 1 - layers[ OUTPUT ][ out_idx ].out;
      }
      else
      {
         layers[ OUTPUT ][ out_idx ].error = -1 - layers[ OUTPUT ][ out_idx ].out;
      }
      
      curr_error += layers[ OUTPUT ][ out_idx ].error;
   }
   
   return curr_error;
}

//////////////////
// Author: Brian Fehrman
// Sigmoid activation function, -1 to 1
// Can easily be changed to other functions
// but need to remember to change the derivative
// function too if training.
///////////////////
DEC_TYPE ANN::compute_f( const DEC_TYPE in_val )
{
   return ( ( exp( in_val ) - exp( -in_val ) ) / ( exp( in_val ) + exp( -in_val ) ) );
}

///////////////////////
// Author: Brian Fehrman
// Computes the derivative of the sigmoid function using the
// output value of the sigmoid function. MUST BE THE OUTPUT
// VALUE OF THE SIGMOID FUNCTION
////////////////////////
DEC_TYPE ANN::compute_f_deriv( const DEC_TYPE f_out_val )
{
   return ( 1 - f_out_val * f_out_val );
}

////////////////////
// Author: Brian Fehrman
// Computes the feed forward between two layers
//////////////////////
void ANN::compute_forward( const perceptron src[], perceptron dst[], 
                           const weight src_to_dst_weights[][MAXOUT + 1], 
                           const int src_size, const int dst_size )
{
   int dst_idx = 0;
   int src_idx = 0;
   
   //for each destination node
   for( dst_idx = 0; dst_idx < dst_size; dst_idx++ )
   {
      dst[ dst_idx ].in = 0.0;
   
      //for each source node
      for( src_idx = 0; src_idx < src_size; src_idx++ )
      {
         dst[ dst_idx ].in += src[ src_idx ].out *
                              src_to_dst_weights[ dst_idx ][ src_idx ].curr_w;
      }
      
      //bias weight
      dst[ dst_idx ].in += -1.0 *  src_to_dst_weights[ dst_idx ][ src_size ].curr_w;
      
      dst[ dst_idx ].out = compute_f( dst[ dst_idx ].in );
      dst[ dst_idx ].out_prime = compute_f_deriv( dst[ dst_idx ].out );
   }
}

/////////////////////////////
// Author: Brian Fehrman
// determines the current output classification
// based on which output node has the max
// output value
////////////////////////////
int ANN::find_output_class()
{
   int curr_class = 0;
   float max_response = -10000;
   int out_idx = 0;
   
   //For each output node
   for( out_idx = 0; out_idx < MAXOUT; out_idx++ )
   {
      if( layers[ OUTPUT ][ out_idx ].out > max_response )
      {
         max_response = layers[ OUTPUT ][ out_idx ].out;
         curr_class = out_idx;
      }
   }
   
   return curr_class;
}

/////////////////
// Author: Brian Fehrman
// Initializes the class members for ANN. Opens weights and feature
// files, also sets max epochs and error tolerance metric
////////////////
int ANN::initialize()
{
   //No point in doing anything else if weight file doesn't open,
   //so try that first  
   w_file_in.open( WEIGHTS_IN );
   
   if( !w_file_in )
   {
      cout << "Error opening file for reading: " << WEIGHTS_IN << endl;
      return -1;
   }
   
   //No point in doing anything else if feature file doesn't open,
   //so try that second 
   f_file_in.open( FV_IN );
   
   if( !f_file_in )
   {
      cout << "Error opening file for reading: " << FV_IN << endl;
      return -1;
   }
   
   
   w_file_out.open( WEIGHTS_OUT );
   
   if( !w_file_out )
   {
      cout << "Error opening file for writing: " << WEIGHTS_OUT << endl;
      return -1;
   }
   
   
   max_epochs = 3000;
   tol = 0.01;
   
   return 0;
}

//////////////////////////
// Author: Brian Fehrman
// Sets the outputs of the input layer to just be
// the current feature values. Makes for easy re-use
// of the feedforward code. I did find a use for
// the input "layer"! =)
//////////////////////////
void ANN::input_layer_init( const DEC_TYPE f_vals[] )
{
   int f_val_idx = 0;
   
   //For each feature value
   for( f_val_idx = 0; f_val_idx < MAXIN + 1; f_val_idx++ )
   {
      layers[ INPUT ][ f_val_idx ].out = f_vals[ f_val_idx ];
   }
}

////////////////////////////
// Author: Brian Fehrman
// Normalizes each of the feature values based on the
// maximum value across all of those features
////////////////////////////
void ANN::normalize_feat( feat_vec& curr_feat )
{
   unsigned int f_val_idx = 0;
   
   for( f_val_idx = 0; f_val_idx < MAXIN + 1; f_val_idx++ )
   {
      //curr_feat.f_vals[ f_val_idx ] /= curr_feat.max_val;
   }
}

/////////////////////
// Author: Brian Fehrman
// Reads in all of the data for
// the desired set of feature vectors (e.g., TRAIN, TEST)
//
// f_set - desired feature vector set
///////////////////////
void ANN::read_feature_set( const int f_set )
{
   DEC_TYPE curr_val = 0.0;
   int f_val_idx = 0;
   int f_vec_idx = 0;
   
   //Read in the training features
   for( f_vec_idx = 0; f_vec_idx < NUMUV; f_vec_idx++ )
   {
      //Read in class of current feature
      f_file_in >> input_vecs[ f_set ][ f_vec_idx ].class_num;
      
      //If FONT_DATA flag is set, read in the font type
      //and increment the starting feat_val index
      if( FONT_DATA )
      {
         f_file_in >> input_vecs[ f_set ][ f_vec_idx ].font_type;
      }

      //For each feature value
      for( f_val_idx = 0; f_val_idx < MAXIN; f_val_idx++ )
      {
         f_file_in >> curr_val;
         input_vecs[ f_set][ f_vec_idx ].f_vals[ f_val_idx ] = curr_val;
      }
      
      //bias input. I make it the last value because it makes indexing
      //easier for me
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
void ANN::read_weight_set( const int w_set, const int src_size, const int dst_size )
{
   int dst_idx = 0;
   int src_idx = 0;
   
   //For each destination node
   for( dst_idx = 0; dst_idx < dst_size; dst_idx++ )
   {
      //For each source node
      for( src_idx = 0; src_idx < src_size; src_idx++ )
      {
         w_file_in >> weights[ w_set ][ dst_idx ][ src_idx ].curr_w;
      }
   }
}

/////////////////
// Author: Brian Fehrman
// Run method for the ANN class. Will call
// methods as needed to run the ANN.
////////////////////////
int ANN::run( )
{
   int curr_train = 0;
   
   initialize( ); 
   
   for( curr_train = 0; curr_train < N_ITERATIONS; curr_train++ )
   {
      train( );
   }
   
   test( );
   
   f_file_in.close( );
   w_file_in.close( );
   w_file_out.close( );
   
   return 0;
}

void ANN::test( )
{
   int curr_class = 0;
   int f_vec_idx = 0;
   int num_right = 0;
   int num_wrong = 0;
   
   //read in weights for each layer pair
   read_feature_set( TEST );
   read_weight_set( I_TO_H, MAXIN + 1, TEST );
   read_weight_set( H_TO_O, MAXH + 1, TEST );
   
   //For each feature vector
   for( f_vec_idx = 0; f_vec_idx < NUMUV; f_vec_idx++ )
   {
      //Setup the feature values to be the output of the input
      //layer. Allows for reuse of code for the compute
      //forward function
      input_layer_init( input_vecs[ TEST ][ f_vec_idx ].f_vals );

      //Compute feed forwad from input to hidden layer
      compute_forward( layers[ INPUT ], layers[ HIDDEN ], weights[ I_TO_H ], MAXIN, MAXH );
              
      //Compute feed forward from hidden to output layer
      compute_forward( layers[ HIDDEN ], layers[ OUTPUT ], weights[ H_TO_O ], MAXH, MAXOUT );
      
      curr_class = find_output_class();
      
      //Update classification stats
      if( curr_class == input_vecs[ TEST ][ f_vec_idx ].class_num )
      {
         num_right++;
      }
      else
      {
         num_wrong++;
      }
      
      cout << num_right << " " << num_wrong << endl;
   }
}

////////////////////
// Author: Brian Fehrman
// trains the weights for the network
// using backpropogation. Momentum is implemented
// in the update functions
////////////////////
void ANN::train( )
{
   int curr_class = 0;
   int curr_epoch = 0;
   int f_vec_idx = 0;
   int num_right = 0;
   int num_wrong = 100;
   
   //read in weights for each layer pair
   read_feature_set( TRAIN );
   read_weight_set( I_TO_H, MAXIN + 1, MAXH );
   read_weight_set( H_TO_O, MAXH + 1, MAXOUT );
   
   curr_epoch_error = 100000;
   
   //While we don't have them all right or we reach max_epochs
   while( curr_epoch_error > 0.0001  && curr_epoch < max_epochs)
   {
      curr_epoch_error = 0;
      num_right = 0;
      num_wrong = 0;
      
      //For each feature vector
      for( f_vec_idx = 0; f_vec_idx < NUMUV; f_vec_idx++ )
      {
         //Setup the feature values to be the output of the input
         //layer. Allows for reuse of code for the compute
         //forward function
         input_layer_init( input_vecs[ TRAIN ][ f_vec_idx ].f_vals );

         //Compute feed forwad from input to hidden layer
         compute_forward( layers[ INPUT ], layers[ HIDDEN ], weights[ I_TO_H ], MAXIN, MAXH );
                 
         //Compute feed forward from hidden to output layer
         compute_forward( layers[ HIDDEN ], layers[ OUTPUT ], weights[ H_TO_O ], MAXH, MAXOUT );
         
         //Determine the current error and add it to the current epoch error;
         curr_epoch_error += abs( compute_error( input_vecs[ TRAIN ][ f_vec_idx ].class_num ) );
         
         //Compute the del values
         compute_del( );
         
         //Update weights from input to hidden layer
         update_i_to_h_weights( );
         
         //Update hidden to output
         update_h_to_o_weights( );
         
         //Determine what the network thinks the current class is
         curr_class = find_output_class( );

         //Update classification stats
         if( curr_class == input_vecs[ TRAIN ][ f_vec_idx ].class_num )
         {
            num_right++;
         }
         else
         {
            num_wrong++;
         }
      }
      
      if( curr_epoch % LOG_MOD == 0 )
      {

      }
      
      curr_epoch++;
   }
   
   //Write the input to hidden weight set
   write_weights( I_TO_H, MAXIN + 1, MAXH );
   
   //Write the hidden to output weight set
   write_weights( H_TO_O, MAXH + 1, MAXOUT );
   
}

///////////////////////
// Author: Brian Fehrman
// Updates weights from hidden
// to output. Uses momentum.
////////////////////////
void ANN::update_h_to_o_weights( )
{
   int h_idx = 0;
   int out_idx = 0;
   
   //For each output node
   for( out_idx = 0; out_idx < MAXOUT; out_idx++ )
   {
      //For each hidden node
      for( h_idx = 0; h_idx < MAXH + 1; h_idx++ )
      {
         weights[ H_TO_O ][ out_idx ][ h_idx ].prev_w = weights[ H_TO_O ][ out_idx ][ h_idx ].curr_w;
         
         weights[ H_TO_O ][ out_idx ][ h_idx ].curr_w = weights[ H_TO_O ][ out_idx ][ h_idx ].prev_w +
                                                      layers[ OUTPUT ][ out_idx ].error *
                                                      layers[ OUTPUT ][ out_idx ].out_prime *
                                                      layers[ HIDDEN ][ h_idx ].out * 
                                                      ETA + ALPHA * weights[ H_TO_O ][ out_idx ][ h_idx ].prev_change;
                                                      
         //Update change in weights which is used for momentum
         weights[ H_TO_O ][ out_idx ][ h_idx ].prev_change = weights[ H_TO_O ][ out_idx ][ h_idx ].curr_w - 
                                                            weights[ H_TO_O ][ out_idx ][ h_idx ].prev_w;
      }
   }
}

///////////////////////
// Author: Brian Fehrman
// Updates weights from input
// to hidden. Uses momentum.
////////////////////////
void ANN::update_i_to_h_weights( )
{
   int h_idx = 0;
   int in_idx = 0;
   
   //For each hidden node
   for( h_idx = 0; h_idx < MAXH; h_idx++ )
   {
      //For each input node
      for( in_idx = 0; in_idx < MAXIN + 1; in_idx++ )
      {
         weights[ I_TO_H ][ h_idx ][ in_idx ].prev_w = weights[ I_TO_H ][ h_idx ][ in_idx ].curr_w;
         
         weights[ I_TO_H ][ h_idx ][ in_idx ].curr_w = weights[ I_TO_H ][ h_idx ][ in_idx ].prev_w +
                                                      layers[ INPUT ][ in_idx ].out * 
                                                      layers[ HIDDEN ][ h_idx ].out_prime *
                                                      del[ h_idx ] * ETA +  
                                                      ALPHA * weights[ I_TO_H ][ h_idx ][ in_idx ].prev_change;
         
         //Update change in weights which is used for momentum
         weights[ I_TO_H ][ h_idx ][ in_idx ].prev_change = weights[ I_TO_H ][ h_idx ][ in_idx ].curr_w - 
                                                            weights[ I_TO_H ][ h_idx ][ in_idx ].prev_w;
      }
   }
}

//////////////////////////////
// Author: Brian Fehrman
// writes out the trained weights for the given set
//////////////////////////////
int ANN::write_weights( const int w_set, const int src_size, const int dst_size )
{
   int dst_idx = 0;
   int src_idx = 0;
   
   w_file_out << setprecision( 6 ) << fixed;
   
   //For each destination node
   for( dst_idx = 0; dst_idx < dst_size; dst_idx++ )
   {
      //For each source node
      for( src_idx = 0; src_idx < src_size; src_idx++ )
      {
         w_file_out << setw( 10 ) << weights[ w_set ][ dst_idx ][ src_idx ].curr_w;
         
         //Try to keep looking like you had it
         if( ( src_idx + 1 ) % 7 == 0 )
         {
            w_file_out << endl;
         }
      }
      
      w_file_out << endl;
   }
   
   w_file_out << endl;
   
   return 0;
}
