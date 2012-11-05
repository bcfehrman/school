#ifndef _MAHALANOBIS_CLASSIFIER_H
#define _MAHALANOBIS_CLASSIFIER_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;

/****** Structs ******/
struct mahalanobis_class_data
{
   vector< vector<float> > covariance_matrix;
   vector<float> means;
   int num_samples;
};

struct mahalanobis_data_point
{
   vector<float> features;
   int class_num;
};

class mahalanobis_classifier
{
   private:
   
      /*** Private Members ***/
      vector<mahalanobis_class_data> classifier_data;
      string data_file_name;
      vector<mahalanobis_data_point> data_set;
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
      mahalanobis_classifier( );
      mahalanobis_classifier( string data_file_name, int num_classes, int num_class_features );
      
      /*** Public Methods ***/
      int run();
   
};


#endif
