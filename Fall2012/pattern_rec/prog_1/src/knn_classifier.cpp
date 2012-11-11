#include "knn_classifier.h"

/***** Constructors ***////

////////////////////////
//Empty default constructor for knn classifier
////////////////////////
knn_classifier::knn_classifier()
{
   
}

////////////////////////
//Author: Brian Fehrman
//Main constructor for the knn classifier class
//Sets the class variables using the given parameters
////////////////////////
knn_classifier::knn_classifier( string data_file_name, int num_classes, int num_class_features, int num_neighbors )
{
   this->data_file_name = data_file_name;
   this->num_classes = num_classes;
   this->num_class_features = num_class_features;
   this->num_neighbors = num_neighbors;
   num_right = 0;
   num_wrong = 0;
   num_data_points = 0;
   
   //Create place for each class
   classifier_data.resize( num_classes );
   
   for( int i = 0; i < num_classes; i++ )
   {
      classifier_data[ i ].num_samples = 0;
      classifier_data[ i ].num_right = 0;
      classifier_data[ i ].num_wrong = 0;      
   }
}


/***** Functions ****/

////////////////////////
//Author: Brian Fehrman
//Perfoms classification on the second half of the
//data set (the testing data). For each data point,
//the euclidean distance between that point's features and the
//features of every data point from the training set (the first half
//of the data set) is computed. The class which produces the minimum
//distance between features is chosen as the "classification" for the
//data point. For k > 1, a "vote" is taken by the top k smallest class
//distances and the majority class is chosen as the "classification".
//This is done for all data points in the testing set. The function keeps
//a running total of how many it gets wrong and right and outputs
//these results. 
////////////////////////
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
   
   //Setup the vector used for keeping track of the "vote"
   //among closest classes
   for( curr_class = 0; curr_class < num_classes; curr_class++ )
   {
      closest_class_tally.push_back( 0 );
   }
   
   //Classify the points
   for( int i = num_data_points / 2; i < num_data_points; i++ )
   {
      //Reset the values in the tally vector
      for( curr_class = 0; curr_class < num_classes; curr_class++ )
      {
         closest_class_tally[ curr_class ] = 0;
      }
      
      //Clear the vector of closest matches and start with a clean slate
      closest_matches.clear();
      min_dist_class = 0;
      
      //Set starting values in closest match vector to be very large
      for( int k  = 0; k < num_neighbors; k++ )
      {
            curr_close_match.distance = 100000000;
            closest_matches.push_back( curr_close_match );
      }
      
      //Find euclidean distance between current data point's features
      //and the features from every data point in the training set (first
      //half of the data set)
      for( int j = 0; j < num_data_points / 2; j++ )
      {
         //Reset current distance
         curr_dist = 0;
         
         //Compute euclidean distance between features and sum
         for(  int curr_feat = 0; curr_feat < num_class_features; curr_feat++ )
         {
            curr_temp_dist = ( data_set[ i ].features[ curr_feat ] - data_set[ j ].features[ curr_feat ] );
            curr_temp_dist *= curr_temp_dist;
            
            curr_dist += curr_temp_dist;
         }
         
         //Square root of sum of squared differences. Not sure if I need/want this
         curr_dist = sqrt(curr_dist);
         
         //Traverse through the list of closest matches and insert the current
         //distance and class in to the list at the correct position in the
         //list (ascending order). If an insertation takes place, then the
         //last element of the vector is erased so that it's length remains
         //the same as that of the desired number of neighbors. This effectively
         //keeps track of the top k smallest distances and the classes that produced them
         for( int k = 0; k < num_neighbors; k++ )
         {
            if( curr_dist < closest_matches[ k ].distance )
            {
               curr_close_match.distance = curr_dist;
               curr_close_match.class_num = data_set[ j ].class_num;
               closest_matches.insert( closest_matches.begin() + k, curr_close_match );
               closest_matches.erase( closest_matches.begin() + num_neighbors );
               break;
            }
         }
      }
      
      //Reset the max tally
      max_tally = 0;
      
      //Run through the k smallest distances chosen and take a "vote" of
      //the classes that produced those distances. Current data point
      //is classified as the majority class
      for(  int k = 0; k < num_neighbors; k++ )
      {
         closest_class_tally[ closest_matches[ k ].class_num ]++;
         
         //Check if we have a new majority class and update if so
         if( closest_class_tally[ closest_matches[ k ].class_num ] > max_tally )
         {
            max_tally = closest_class_tally[ closest_matches[ k ].class_num ];
            min_dist_class = closest_matches[ k ].class_num;
         }
      }
      
      //Tally how many data points are correctly/incorrectly classified
      if( data_set[ i ].class_num == min_dist_class )
      {
         num_right++;
         classifier_data[ data_set[ i ].class_num ].num_right++;
      }
      else
      {
         num_wrong++;
         classifier_data[ data_set[ i ].class_num ].num_wrong++;
      }
      
      //Left in for debugging and examination purposes. Can uncomment if needed
      //cout << "Decided current feature is class: " << min_dist_class << " and it is really class: " << data_set[ i ].class_num << endl;
   }
   
   for( int i = 0; i < num_classes; i++ )
   {
      cout << "Class : " << i << " had " << classifier_data[ i ].num_right << " right and " << classifier_data[ i ].num_wrong << " wrong." << endl;
   }
   cout << endl;
   cout << "Total number right: " << num_right << " Total Number wrong: " << num_wrong << endl;
}

//////////////////////////
//Author: Brian Fehrman
//Reads data from a white space seperated values file.
//Expects that on each line, there will be features given.
//Following the features (last value on line) should be
//the class for the given data point
/////////////////////////
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
   
   //Total number of data points
   num_data_points = data_set.size();
   
   fin.close();
   
   return 0;
}

////////////////////////////
//Author: Brian Fehrman
//Just the encapsulating run function
//for the class which will call all of the
//functions necessary to perform knn
//classification on a data set. Uses the first
//half of the given set for training and the second
//half for testing
////////////////////////////
int knn_classifier::run()
{
   cout << endl << setw(50) <<"***RUNNING KNN CLASSIFIER***" << endl << endl;
   cout << setw(80) << setfill('^') << " ";
   cout << setfill(' ') << endl;
   
   cout << "Using " << num_neighbors << " neighbors" << endl << endl;
   
   //Read in file data and return error
   //if unsucceseful
   if( read_file_data() )
      return -1;
   
   //Perform the classification of the data
   classify_data();
   
   cout << endl << setw(50)<< "***FINISHED KNN CLASSIFIER***" << endl << endl;
   cout << setw(80) << setfill('-') << " ";
   cout << setfill(' ') << endl;
   
   return 0;
}
