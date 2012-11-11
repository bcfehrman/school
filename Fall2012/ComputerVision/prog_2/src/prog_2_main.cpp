
/**
Author: Brian Fehrman
Class: Computer Vision Fall 2012
Professor: Dr. Hoover
Assign: Program 2

**/

/***** Includes *****/
#include "prog_2.h"

vector<Point> chosen_points;
/******** Main ***********/

void my_mouse_callback( int event, int x, int y, int flags, void* param );

int main( int argc, char *argv[])
{  
   SurfFeatureDetector sfd;
   Mat image_in;
   stringstream converter;
   namedWindow("yo", WINDOW_SIZE_CHOICE );
   cvSetMouseCallback( "yo", my_mouse_callback );
   Mat image_1, image_1_highlight;
   Mat image_2, image_2_highlight;
   int key_pressed = 'a';
   Mat combined_images;
   image_1 = imread("img/mine/buzzard1.jpg");
   image_2 = imread("img/mine/buzzard2.jpg");
   int fontFace = FONT_HERSHEY_DUPLEX;
   Scalar colors[4] = {Scalar(0,220,220), Scalar(220,0,220), Scalar(220,220,0), Scalar(50,220,50) }; 
   
   
   /*
   //Perform the SVD that gives the U, W, Vt (or U, S, Vt) except W is just a vector of the singular values
   SVD svd(im1_gray);

   cout << svd.u.rows << " " << svd.u.cols << endl;

   cout << svd.w.rows << " " << svd.w.cols << endl;

   cout << svd.vt.rows << " " << svd.vt.cols << endl;
   */
   
   for(;;)
   {
      image_1.copyTo( image_1_highlight );
      
      for(unsigned int i = 0; i < chosen_points.size(); i++ )
      {

         circle(image_1_highlight, chosen_points[ i ], 10, colors[i % 4], -1);
         converter << i % 4;
         putText (image_1_highlight,converter.str(),chosen_points[ i ], fontFace, 2, colors[i % 4], 3);
         converter.str("");
         converter.clear();

      }
      imshow("yo", image_1_highlight);
      
      key_pressed = (int) waitKey( 30 );
      
      if( key_pressed >= 0)
      {
         if(  ( char )key_pressed == ' ' )
         {
            
         }
         else
         {
            break;
         }
      } 
   }

   return 0;
}

void my_mouse_callback( int event, int x, int y, int flags, void* param ){

switch( event ){
    case CV_EVENT_LBUTTONDOWN:
        cout << " x: " << x << " y: " << y << endl;
        
        if( chosen_points.size() < 8 )
        {
           chosen_points.push_back(Point( x, y ) );
        }
        break;

    case CV_EVENT_RBUTTONDOWN:
        if( chosen_points.size() > 0 )
        {
           chosen_points.erase( chosen_points.end() );
        }
        break;

    case CV_EVENT_MBUTTONDOWN:
        cout << "Perform homography" << endl;
        break;
     }
}
