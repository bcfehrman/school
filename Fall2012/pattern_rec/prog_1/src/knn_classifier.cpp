#include "knn_classifier.h"

/***** Constructors ***////

knn_classifier::knn_classifier()
{
   
}

knn_classifier::knn_classifier( string data_file_name, int num_classes, int num_class_features, int num_neighbors )
{
   this->data_file_name = data_file_name;
   this->num_classes = num_classes;
   this->num_class_features = num_class_features;
   this->num_neighbors = num_neighbors;
   num_right = 0;
   num_wrong = 0;
   num_data_points = 0;
}


/***** Functions ****/

void knn_classifier::classify_data()
{
   vector<close_match> closest_matches;
   vector<int> closest_class_tally;
   close_match curr_close_match;
   int curr_class = 0;
   float curr_dist = 0.0;
   float curr_temp_dist = 0.0;
   int max_tally = 0;
   int min_dist_class = 0;
   
   cout << endl << "***CLASSIFIYING DATA***" << endl << endl;
   
   for( curr_class = 0; curr_class < num_classes; curr_class++ )
   {
      closest_class_tally.push_back( 0 );
   }
   
   //Classify the points
   for( int i = num_data_points / 2; i < num_data_points; i++ )
   {
      closest_matches.clear();
      min_dist_class = 0;
      
      for( int k  = 0; k < num_neighbors; k++ )
      {
            curr_close_match.distance = 10000000;
            closest_matches.push_back( curr_close_match );
      }
      
      for( int j = 0; j < num_data_points / 2; j++ )
      {
         curr_dist = 0;
         
         for(  int curr_feat = 0; curr_feat < num_class_features; curr_feat++ )
         {
            curr_temp_dist = ( data_set[ i ].features[ curr_feat ] - data_set[ j ].features[ curr_feat ] );
            curr_temp_dist *= curr_temp_dist;
            
            curr_dist += curr_temp_dist;
         }
         
         for( int k = 0; k < num_neighbors; k++ )
         {
            if( curr_dist < closest_matches[ k ].distance )
            {
               closest_matches[ k ].distance = curr_dist;
               closest_matches[ k ].class_num = data_set[ j ].class_num;
               break;
            }
         }
      }
      
      max_tally = 0;
      
      for(  int k = 0; k < num_neighbors; k++ )
      {
         closest_class_tally[ closest_matches[ k ].class_num ]++;
         
         if( closest_class_tally[ closest_matches[ k ].class_num ] > max_tally )
         {
            max_tally = closest_class_tally[ closest_matches[ k ].class_num ];
            min_dist_class = closest_matches[ k ].class_num;
         }
      }
      
      if( data_set[ i ].class_num == min_dist_class )
      {
         num_right++;
      }
      else
      {
         num_wrong++;
      }
      
      //Left in for debugging and examination purposes. Can uncomment if needed
      //cout << "Decided current feature is class: " << min_dist_class << " and it is really class: " << data_set[ i ].class_num << endl;
   }
   
   cout << "Number right: " << num_right << " Number wrong: " << num_wrong << endl;
}



int knn_classifier::read_file_data()
{
   ifstream fin;
   float curr_feat;
   knn_data_point curr_data_point;
   
   cout << endl << "***READING FILE DATA***" << endl << endl;
   
   //Attempt to open data file and exit with error if unable to
   fin.open( data_file_name.c_str() );
   if( !fin )
   {
      cout << "Could not open file: " << data_file_name.c_str() << endl;
      cout << "Exiting..." << endl << endl;
      
      return -1;
   }
   else
   {
      cout << "File read succesful!" << endl;
   }
   
   //Loop through file and get all features for each data point, plus
   //the class for that data point
   while( fin >> curr_feat )
   {
      curr_data_point.features.push_back( curr_feat );
      
      //Assume that first data item on line is a feature
      //and then loop through the rest of the features
      for( int i = 0; i < num_class_features - 1; i++ )
      {
         fin >> curr_feat;
         curr_data_point.features.push_back( curr_feat );
      }
      
      //Assume last data item on line is the class
      fin >> curr_data_point.class_num;
      
      //Add current data point to the data set
      data_set.push_back( curr_data_point );
      
      //Clear features in our temp data point holder
      curr_data_point.features.clear();
   }
   
   num_data_points = data_set.size();
   
   fin.close();
   
   return 0;
}

int knn_classifier::run()
{
   cout << endl << setw(50) <<"***RUNNING KNN CLASSIFIER***" << endl << endl;
   cout << setw(80) << setfill('^') << " ";
   cout << setfill(' ');
   
   cout << "Using " << num_neighbors << " neighbors" << endl << endl;
   
   //Read in file data and return error
   //if unsucceseful
   if( read_file_data() )
      return -1;
   
   classify_data();
   
   cout << endl << setw(50)<< "***FINISHED KNN CLASSIFIER***" << endl << endl;
   cout << setw(80) << setfill('-') << " ";
   cout << setfill(' ') << endl;
   
   return 0;
}
