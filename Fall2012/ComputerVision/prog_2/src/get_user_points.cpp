/*
 * get_user_points.cpp
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 * 
 */


#include "get_user_points.h"


get_user_points::get_user_points()
{

}


get_user_points::~get_user_points()
{
   
}

void get_user_points::get_points( Mat& image, vector<Vec3d>& chosen_points, unsigned int num_points )
{
   vector<Scalar> colors; 
   stringstream converter;
   int fontFace = FONT_HERSHEY_DUPLEX;
   int key_pressed = 'a';
   Mat image_highlight;
      
   this->num_points = num_points;   
   this->chosen_points_ptr = chosen_points;
      
   chosen_points.clear();
   
   for( unsigned int curr_color = 0; curr_color < num_points; curr_color++ )
   {
      colors.push_back( Scalar( ( curr_color + 1 ) * 50 % 255, ( curr_color + 1 ) * 25 % 255, ( curr_color + 1 ) * 75 % 255) );  
   }
   
   namedWindow("Pick corresponding points", WINDOW_SIZE_CHOICE );
   cvSetMouseCallback( "Pick corresponding points", mouse_callback, this );
   
   for(;;)
   {
      image.copyTo( image_highlight );
      
      for(unsigned int i = 0; i < chosen_points.size(); i++ )
      {

         circle(image_highlight, chosen_points[ i ], 10, colors[i % 4], -1);
         converter << i % 4;
         putText (image_highlight,converter.str(),chosen_points[ i ], fontFace, 2, colors[i % 4], 3);
         converter.str("");
         converter.clear();

      }
      imshow("Pick corresponding points", combined_images_highlight);
      
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
}

////////////////////////
// Author: Brian Fehrman
// Callback method for when the mouse is clicked on
// the image display window in order to select correspondence points.
// A left click selects a point. First 4 points are selected, with the
// selections being numbered and colored. Then 4 more points are selected
// with corresponding colors and numbers being displayed.
/////////////////////////

void get_user_points::mouse_callback( int event, int x, int y, int flags, void* param )
{
   //Static cast is because this is a static method. Just how c++ wanted it...
   get_user_points* chosen_points = static_cast<get_user_points*>(param);
   
   switch( event )
   {
      //Select a point
      case CV_EVENT_LBUTTONDOWN:
        cout << " x: " << x << " y: " << y << endl;
        
        if( get_user_points->chosen_points.size() < 4 )
        {
           get_user_points->chosen_points.push_back( Vec3d( x, y, 1.0 ) );
        }
        break;
      
      //Deselect a point
      case CV_EVENT_RBUTTONDOWN:
        if( get_user_points->chosen_points.size() > 0 )
        {
           get_user_points->chosen_points.erase( chosen_points.end() );
        }
        break;

      //Perform the homography
      case CV_EVENT_MBUTTONDOWN:
        cout << "Perform homography" << endl;
        break;
   }
}
