#ifndef _GEN_NORMAL_H
#define _GEN_NORMAL_H

#include<cmath>
#include<vector>
#include<fstream>
#include<iostream>
#include<string>
#include <cstdlib>

using namespace std;

/****** Structs ******/
struct norm_data_point
{
   vector<float> means;
   vector<float> standard_deviations;
   int class_num;
};


class gen_normal
{
   private:
   
      /*** Private Members ***/
      vector<norm_data_point> class_data;
      string file_out_name;
      int num_classes;
      int num_class_features;
      int pop_size;
      
      /*** Private Methods ***/

   
   public:
      
      /***Constructors***/
      gen_normal( );
      gen_normal( string file_out_name, int num_classes, int num_class_features, 
                  int pop_size, float mean_bound, float standard_deviation_bound );

      
      /*** Public Methods ***/
      int run();
   
};


#endif
