///////////////////////
//Author: Brian Fehrman
//Class: Fall 2012 Pattern Recognition
//Teacher: Dr. Corwin
//Program: Program 1
//Due Date: 11/9/2012 midnight
//
//This program will take a data file with data points on
//each line which should be whitespace seperated values
//that are in the form "feat_1_val feat_2_val ... feat_n_val class_num".
//Optional command line arguments are number of classes and number
//of features per class. This program will run mahalanobis classfier,
//mle classifier, and knn classifier with k=1 and then k=3. The summary
//of each classifier are displayed in the console.
//
//The program is very close to be able to handle an arbitrary number
//of features for each class but is not quite there and currently
//handles 2. MLE is really the only one that will not work for
//n features -- mahalanobis and knn classifiers should work.
//This just has to do with the determinant term used in the MLE.
/////////////////////

#include "prog_1.h"

int main( int argc, char** argv)
{
   string file_name;
   knn_classifier* knn_classifier_obj;
   mahalanobis_classifier* mh_classifier_obj;
   mle_classifier* mle_classifier_obj;
   int num_classes = 3; //Default number of classes
   int num_class_features = 2; //Default number of features per class

   //Deal with commande line arguments
   switch( argc )
   {
      case 4:
         num_class_features = atoi( argv[ 3 ] );
      
      case 3:
         num_classes = atoi( argv[ 2 ] );
         
      //Minimum number of command line arguments needed
      case 2:
         file_name = argv[ 1 ];
         break;
         
      default:
            cout << "Program usage: ./prog_1 data_file_name (num_classes=3) (num_class_features=2) " << endl;
            return -1;
   }

   //Run the mahalanobis distance classifier
   mh_classifier_obj = new mahalanobis_classifier( file_name, num_classes, num_class_features );
   mh_classifier_obj->run();
   
   //Run the MLE classifier
   mle_classifier_obj = new mle_classifier( file_name, num_classes, num_class_features );
   mle_classifier_obj->run();
   
   //Run the KNN classifier with k = 1
   knn_classifier_obj = new knn_classifier( file_name, num_classes, num_class_features, 1 );
   knn_classifier_obj->run();
   
   //Run the KNN classifier with k = 3
   knn_classifier_obj = new knn_classifier( file_name, num_classes, num_class_features, 3 );
   knn_classifier_obj->run();
   
   return 0;
}
