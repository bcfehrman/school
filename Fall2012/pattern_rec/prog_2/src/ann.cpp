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
   weights[ I_TO_H ].resize( MAXIN + 1 );
   for( unsigned int in_idx = 0; in_idx < weights[ I_TO_H ].size(); in_idx++ )
   {
      weights[ I_TO_H ][ in_idx ].resize( MAXH );
   }
   
   //Create space for hidden to output weights
   weights[ H_TO_O ].resize( MAXH + 1);
   for( unsigned int h_idx = 0; h_idx < weights[ H_TO_O ].size(); h_idx++ )
   {
      weights[ H_TO_O ][ h_idx ].resize( MAXOUT );
   }
   
   //Create space for del vector
   del.resize( MAXOUT );
   
   //Create space for class info vector which tracks classification stats
   //for each class
   class_info.resize( MAXOUT );
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
   int num_read = MAXIN + 1;
   
   if( !fin )
   {
      cout << "Error opening file: " << FV_IN << endl;
      return -1;
   }
   
   //Check if font value is also on the line
   if( FONT_DATA )
   {
      num_read++;
   }

   read_set_features( fin, TRAIN, NUMUV, num_read );
   read_set_features( fin, TEST, NUMUV, num_read );
   
   return 0;
}

/////////////////////
// Author: Brian Fehrman
//
///////////////////////
void ANN::read_set_features( ifstream& fin, const int feat_set, const int num_feats, const int num_f_vals )
{
   DEC_TYPE curr_val = 0.0;
   int feat_val_idx = 1;
   
   //Read in the training features
   for( int feat_idx = 0; feat_idx < num_feats; feat_idx++ )
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
         normalize_feat( input_vecs[ feat_set ][ feat_idx ] );
      }
   }
}

/////////////////
// Author: Brian Fehrman
// Run method for the ANN class. Will call
// methods as needed to run the ANN.
////////////////////////
int ANN::run()
{
   initialize();
   read_all_features();
   
   return 0;
}
