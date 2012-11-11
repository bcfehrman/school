#include "mle_classifier.h"

/***** Constructors ***////

////////////////////////
//Empty default constructor for Mahalanobis classifier
////////////////////////
mle_classifier::mle_classifier()
{
   
}

////////////////////////
//Author: Brian Fehrman
//Main constructor for the mle_classifier class
//Sets the class variables using the given parameters
////////////////////////
mle_classifier::mle_classifier( string data_file_name, int num_classes, int num_class_features )
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
      classifier_data[ i ].num_right = 0;
      classifier_data[ i ].num_wrong = 0;

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
//the mle distance between that point's features
//and the mean of the features for each class is calculated. The class
//which provides that feature means that give the maximum likelihood
//value is chosen as the "classification" for the data point.
//This is done for all data points. The function keeps
//a running total of how many it gets wrong and right and outputs
//these results. 
////////////////////////
void mle_classifier::classify_data()
{
   float curr_det_term = 0.0;
   float curr_data = 0.0;
   float curr_likelihood = 0.0;
   float curr_mean = 0.0;
   float curr_probability = 0.0;
   float curr_variance = 0.0;
   float max_curr_likelihood = 0.0;
   int max_curr_likelihood_class = 0;
   
   cout << endl << "***CLASSIFIYING DATA***" << endl << endl;
   
   //Classify the points
   for( int i = num_data_points / 2; i < num_data_points; i++ )
   {
      //Reset max likelihood to be very small
      max_curr_likelihood = -10000000;
      
      //For each class
      for(  int curr_class = 0; curr_class < num_classes; curr_class++ )
      {
         //Reset current likelihood and get probability for current class
         curr_likelihood = 0.0;
         curr_probability = classifier_data[ curr_class ].probability;
         curr_det_term = classifier_data[ curr_class ].determinant_term;
         
         //For each feature in the class, compute the mle distance
         //between its mean and the corresponding feature value of the current data point.
         //Sum the distances of each feature comparison.
         for( int curr_feat = 0; curr_feat < num_class_features; curr_feat++ )
         {
            curr_data = data_set[ i ].features[ curr_feat ];
            curr_mean = classifier_data[ curr_class ].means[ curr_feat ];
            curr_variance = classifier_data[ curr_class ].covariance_matrix[ curr_feat ][ curr_feat ];
            
            //I'm still not entirely sure that this formula is right for MLE. I took the probability
            //times the normal density function and then took the log of that.
            curr_likelihood += (curr_data - curr_mean) * ( curr_data - curr_mean) * (1/curr_variance) * -0.5 + log(curr_probability) - curr_det_term;
         }
         
         //Check if likelihood for current class is the largest that we have seen.
         //If so then update the max likelihood and the class that produced it
         if( curr_likelihood > max_curr_likelihood )
         {
            max_curr_likelihood = curr_likelihood;
            max_curr_likelihood_class = curr_class;
         }
      }

      //Tally the number of correct and incorrect classifications
      if( data_set[ i ].class_num == max_curr_likelihood_class )
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

///////////////////////////
//Author: Brian Fehrman
//Runs through the first half of the data set (the training half)
//and uses it to determine the mean for each class feature
//as well as the variance for each class feature. Also determines
//the probability for each class
///////////////////////////
void mle_classifier::determine_training_class_info()
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
      classifier_data[ i ].probability = float( curr_num_samples ) / training_data_set_size;
      
      cout << "Class : " << i << " probability is: " << classifier_data[ i ].probability << endl;
      
      classifier_data[ i ].determinant_term = 1.0;
      
      for( int curr_feat = 0; curr_feat < num_class_features; curr_feat++ )
      {
         //Make sure there is data available for the given class before proceeding
         //to finalize the mean and standard deviations
         if( curr_num_samples > 0 )
         {
            classifier_data[ i ].means[ curr_feat ] /= curr_num_samples;
            
            //This formula is from what we were given in class for calculation variance
            classifier_data[ i ].covariance_matrix[ curr_feat ][ curr_feat ] /= curr_num_samples;
            classifier_data[ i ].covariance_matrix[ curr_feat ][ curr_feat ] -= 
                     classifier_data[ i ].means[ curr_feat ] * classifier_data[ i ].means[ curr_feat ];
            classifier_data[ i ].determinant_term *= classifier_data[ i ].covariance_matrix[ curr_feat ][ curr_feat ];
            
            cout << "Class : " << i << " feature : " << curr_feat << " mean is: " << classifier_data[ i ].means[ curr_feat ] << 
            " variance is: " << classifier_data[ i ].covariance_matrix[ curr_feat ][ curr_feat ] << endl;
         }
         else
         {
            cout << "No data for class: " << i << endl;
         }
      }
      
      cout << endl;
      
      //Used in determining MLE
      classifier_data[ i ].determinant_term = log( sqrt( classifier_data[ i ].determinant_term ) );
      
   }
}

//////////////////////////
//Author: Brian Fehrman
//Reads data from a white space seperated values file.
//Expects that on each line, there will be features given.
//Following the features (last value on line) should be
//the class for the given data point
/////////////////////////
int mle_classifier::read_file_data()
{
   ifstream fin;
   float curr_feat;
   mle_data_point curr_data_point;
   
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
//functions necessary to perform mle
//classification on a data set. Uses the first
//half of the given set for training and the second
//half for testing
////////////////////////////
int mle_classifier::run()
{
   cout << endl << setw(50) <<"***RUNNING MLE CLASSIFIER***" << endl << endl;
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
   
   cout << endl << setw(50) <<"***FINISHED MLE CLASSIFIER***" << endl << endl;
   cout << setw(80) << setfill('-') << " ";
   cout << setfill(' ') << endl;
   
   return 0;
}
