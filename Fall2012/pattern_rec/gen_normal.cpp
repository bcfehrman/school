#include"gen_normal.h"

gen_normal::gen_normal( )
{
   
}

gen_normal::gen_normal( string file_out_name, int num_classes, int num_class_features, int pop_size, float mean_bound, float standard_deviation_bound )
{
   this->file_out_name = file_out_namel
   this->num_classes = num_classes;
   this->num_class_features = num_class_features;
   this->pop_size = pop_size;
   
   srand((unsigned)time(0));
}

gen_normal::gen_normal_pop()
{
   int curr_class = 0;
   ofstream fout;
   
   fout.open( file_out_name.c_str() );
   
   for(int i = 0; i < num_classes; i++ )
   {
      for(int j = 0; j < num_class_featues; j++)
      {
         fout << "Class : " << i << " mean : " << class_data[ i ].means[ j ] <<
               " standard deviation: " << class_data[ i ].standard_deviations[ j ] << endl;
      }
   }
   
   for(int i = 0; i < pop_size; i++ )
   {
      curr_class = rand() % num_classes;
      
      for(int j = 0; j < num_class_features; j++ )
      { 
         
      }
   }
   
}

gen_normal::gen_rand_means()
{
   for( int i = 0; i < num_classes; i++ )
   {
      for( int j = 0; j < num_features; j++ )
      {
         class_data[ i ].means.push_back( rand() % mean_bound )
      }
   }
}

gen_normal::gen_rand_standard_deviations()
{
   for( int i = 0; i < num_classes; i++ )
   {
      for( int j = 0; j < num_features; j++ )
      {
         class_data[ i ].standard_deviations.push_back( rand() % standard_deviation_bound )
      }
   }
}
