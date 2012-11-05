#ifndef _MLE_CLASSIFIER_H
#define _MLE_CLASSIFIER_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;

/****** Structs ******/
struct mle_class_data
{
   vector< vector<float> > covariance_matrix;
   vector<float> means;
   int num_samples;
   float probability;
};

struct mle_data_point
{
   vector<float> features;
   int class_num;
};

class mle_classifier
{
   private:
   
      /*** Private Members ***/
      vector<mle_class_data> classifier_data;
      string data_file_name;
      vector<mle_data_point> data_set;
      int num_class_features;
      int num_classes;
      int num_data_points;
      int num_right;
      int num_wrong;
      
      /*** Private Methods ***/
      void classify_data();
      void determine_training_class_info();
      int read_file_data();
   
   
   public:
      
      /***Constructors***/
      mle_classifier( );
      mle_classifier( string data_file_name, int num_classes, int num_class_features );
      
      /*** Public Methods ***/
      int run();
   
};


#endif
