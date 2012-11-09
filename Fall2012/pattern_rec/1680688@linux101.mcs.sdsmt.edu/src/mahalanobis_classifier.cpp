#include "mahalanobis_classifier.h"

/***** Constructors ***////

////////////////////////
//Empty default constructor for Mahalanobis classifier
////////////////////////
mahalanobis_classifier::mahalanobis_classifier()
{
   
}

////////////////////////
//Author: Brian Fehrman
//Main constructor for the mahalanobis_classifier class
//Sets the class variables using the given parameters
////////////////////////
mahalanobis_classifier::mahalanobis_classifier( string data_file_name, int num_classes, int num_class_features )
{
   this->data_file_name = data_file_name;
   this->num_classes = num_classes;
   this->num_class_features = num_class_features;
   num_right = 0;
   num_wrong = 0;
   num_data_points = 0;

   //Create place for each class
   classifier_data.resize( num_classes );

   //Create place for each feature and setup
   //the covariance matrix size.
   //c++ automatically sets the vector elements to 0
   for( int i = 0; i < num_classes; i++ )
   {
      classifier_data[ i ].num_samples = 0;

      classifier_data[ i ].means.resize( num_class_features );

      classifier_data[ i ].covariance_matrix.resize( num_class_features );

      for( int j = 0; j < num_class_features; j++ )
      {
         classifier_data[ i ].covariance_matrix[ j ].resize( num_class_features );
      }
   }
 }


/***** Functions ****/

////////////////////////
//Author: Brian Fehrman
//Perfoms classification on the second half of the
//data set (the testing data). For each data point,
//the mahalanobis distance between that point's features
//and the mean of the features for each class is calculated. The class
//which provides that feature means that give the smallest
//value is chosen as the "classification" for the data point.
//This is done for all data points. The function keeps
//a running total of how many it gets wrong and right and outputs
//these results. 
////////////////////////
void mahalanobis_classifier::classify_data()
{
   float curr_data = 0.0;
   float curr_dist = 0.0;
   float curr_mean = 0.0;
   float curr_variance = 0.0;
   float min_dist = 0.0;
   int min_dist_class = 0;
   
   cout << endl << "***CLASSIFIYING DATA***" << endl << endl;
   
   //Classify the points
   for( int i = num_data_points / 2; i < num_data_points; i++ )
   {
      //Reset mininum distance to be very large
      min_dist = 10000000;
      
      //For each class
      for(  int curr_class = 0; curr_class < num_classes; curr_class++ )
      {
         //Reset the current distance to be 0
         curr_dist = 0.0;
         
         //For each feature in the class, compute the mahalonobis distance
         //between its mean and the corresponding feature value of the current data point.
         //Sum the distances of each feature comparison.
         for( int curr_feat = 0; curr_feat < num_class_features; curr_feat++ )
         {
            curr_data = data_set[ i ].features[ curr_feat ];
            curr_mean = classifier_data[ curr_class ].means[ curr_feat ];
            curr_variance = classifier_data[ curr_class ].covariance_matrix[ curr_feat ][ curr_feat ];
            
            curr_dist += (curr_data - curr_mean) * ( curr_data - curr_mean) * (1/curr_variance);
         }
         
         //Check if distance from current class mean is the smallest we have seen
         //Update it and the minimum distance class if so
         if( curr_dist < min_dist )
         {
            min_dist = curr_dist;
            min_dist_class = curr_class;
         }
      }
      
      //Tally if the classification was right or wrong
      if( data_set[ i ].class_num == min_dist_class )
      {
         num_right++;
      }
      else
      {
         num_wrong++;
      }
      
      //Left in for debugging and algorithm examination purposes. Can uncomment if needed
      //cout << "Decided current feature is class: " << min_dist_class << " and it is really class: " << data_set[ i ].class_num << endl;
   }
   
   cout << "Number right: " << num_right << " Number wrong: " << num_wrong << endl;
}


///////////////////////////
//Author: Brian Fehrman
//Runs through the first half of the data set (the training half)
//and uses it to determine the mean for each class feature
//as well as the variance for each class feature.
///////////////////////////
void mahalanobis_classifier::determine_training_class_info()
{
   int training_data_set_size = num_data_points / 2;
   float curr_feat_val = 0.0;
   int curr_num_samples = 0;
   
   cout << endl << "***DETERMINING TRAINING CLASS INFORMATION***" << endl << endl;
   
   //Go through the first half of the given data and use that
   //for finding the means and variances for each class
   for( int i = 0; i < training_data_set_size; i++ )
   {
      //Keep track of the number of data points for each class
      classifier_data[ data_set[ i ].class_num ].num_samples++;
      
      //Use class number of current data point as the index into the 
      //classifier data vector
      for(  int curr_feat = 0; curr_feat < num_class_features; curr_feat++ )
      {
         curr_feat_val = data_set[ i ].features[ curr_feat ];
         
         //Precomputation of mean and variance. Computation is finalized in the following loop
         classifier_data[ data_set[ i ].class_num ].means[ curr_feat ] += curr_feat_val;
         classifier_data[ data_set[ i ].class_num ].covariance_matrix[ curr_feat ][ curr_feat ] += curr_feat_val * curr_feat_val;
      }
   }
   
   //Set up the output precision
   cout << fixed << setprecision( 5 );
   
   //Determine mean and variance for each class
   for( int i = 0; i < num_classes; i++ )
   {
      curr_num_samples = classifier_data[ i ].num_samples;
      
      for( int curr_feat = 0; curr_feat < num_class_features; curr_feat++ )
      {
         //Make sure there is data available for the given class before proceeding
         //to finalize the mean and standard deviations
         if( curr_num_samples > 0 )
         {
            classifier_data[ i ].means[ curr_feat ] /= curr_num_samples;
            
            //This formula is from what we were given in class for calculating variance
            classifier_data[ i ].covariance_matrix[ curr_feat ][ curr_feat ] /= curr_num_samples;
            classifier_data[ i ].covariance_matrix[ curr_feat ][ curr_feat ] -= 
                     classifier_data[ i ].means[ curr_feat ] * classifier_data[ i ].means[ curr_feat ];
            
            cout << "Class : " << i << " feature : " << curr_feat << " mean is: " << classifier_data[ i ].means[ curr_feat ] << 
            " variance is: " << classifier_data[ i ].covariance_matrix[ curr_feat ][ curr_feat ] << endl;
         }
         else
         {
            cout << "No data for class: " << i << endl;
         }
      }
   }
}

//////////////////////////
//Author: Brian Fehrman
//Reads data from a white space seperated values file.
//Expects that on each line, there will be features given.
//Following the features (last value on line) should be
//the class for the given data point
/////////////////////////
int mahalanobis_classifier::read_file_data()
{
   ifstream fin;
   float curr_feat;
   mahalanobis_data_point curr_data_point;
   
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
      for( int i = 1; i < num_class_features; i++ )
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
//functions necessary to perform mahalanobis
//classification on a data set. Uses the first
//half of the given set for training and the second
//half for testing
////////////////////////////
int mahalanobis_classifier::run()
{
   cout << endl << setw(55) <<"***RUNNING MAHALANOBIS CLASSIFIER***" << endl << endl;
   cout << setw(80) << setfill('^') << " ";
   cout << setfill(' ');
   
   //Read in file data and return error
   //if unsucceseful
   if( read_file_data() )
      return -1;
     
   //Calculate means, variances, etc. for each class
   determine_training_class_info();
   
   //Perform the classification of the data
   classify_data();
   
   cout << endl << setw(55) <<"***FINISHED MAHALANOBIS CLASSIFIER***" << endl << endl;
   cout << setw(80) << setfill('-') << " ";
   cout << setfill(' ') << endl;
   
   return 0;
}
