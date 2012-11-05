#ifndef _KNN_CLASSIFIER_H
#define _KNN_CLASSIFIER_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;

/****** Structs ******/
struct knn_data_point
{
   vector<float> features;
   int class_num;
};

struct close_match
{
  int class_num;
  float distance; 
};

class knn_classifier
{
   private:
   
      /*** Private Members ***/
      string data_file_name;
      vector<knn_data_point> data_set;
      int num_class_features;
      int num_classes;
      int num_data_points;
      int num_neighbors;
      int num_right;
      int num_wrong;
      
      /*** Private Methods ***/
      void classify_data();
      int read_file_data();
   
   public:
      
      /***Constructors***/
      knn_classifier( );
      knn_classifier( string data_file_name, int num_classes, int num_class_features, int num_neighbors );
      
      /*** Public Methods ***/
      int run();
   
};


#endif
