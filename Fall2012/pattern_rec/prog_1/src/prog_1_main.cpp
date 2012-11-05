#include "prog_1.h"

int main( int argc, char** argv)
{
   string file_name;
   knn_classifier* knn_classifier_obj;
   mahalanobis_classifier* mh_classifier_obj;
   mle_classifier* mle_classifier_obj;
   int num_classes = 3;
   int num_class_features = 2;

   //Deal with commande line arguments
   switch( argc )
   {
      case 4:
         num_class_features = atoi( argv[ 3 ] );
      
      case 3:
         num_classes = atoi( argv[ 2 ] );
         
      case 2:
         file_name = argv[ 1 ];
         break;
         
      default:
            cout << "Program usage: ./prog_1 data_file_name (num_classes) (num_class_features) " << endl;
            return -1;
   }

   mh_classifier_obj = new mahalanobis_classifier( file_name, num_classes, num_class_features );
   mh_classifier_obj->run();
   
   mle_classifier_obj = new mle_classifier( file_name, num_classes, num_class_features );
   mle_classifier_obj->run();
   
   knn_classifier_obj = new knn_classifier( file_name, num_classes, num_class_features, 1 );
   knn_classifier_obj->run();
   
   knn_classifier_obj = new knn_classifier( file_name, num_classes, num_class_features, 3 );
   knn_classifier_obj->run();
   
   return 0;
}
