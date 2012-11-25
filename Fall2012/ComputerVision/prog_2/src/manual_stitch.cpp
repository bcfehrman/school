/*
 * manual_stitch.cpp
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 * 
 */


#include "manual_stitch.h"

/////////// Manual Constructors and Destructors /////////////

manual_stitch::manual_stitch()
   : stitcher()
{
   
}

////////////////////////////
// Author: Brian Fehrman
// Main constructor for the manual class
////////////////////////

manual_stitch::manual_stitch(string p_file_name, string p_prime_file_name, const unsigned int num_points, const unsigned int stitch_type)
   : stitcher(p_file_name, p_prime_file_name, num_points)
{
   this->stitch_type = stitch_type;
   
   if( stitch_type == MANUAL_FRAME_STITCH || stitch_type == MANUAL_MOSAIC_STITCH )
   {
      num_points_get = 8;
   }
   else
   {
      num_points_get = 4;
   }
   
   box_x = 0.0;
   box_y = 0.0;
   
   p_max_x = -100000.0;
   p_max_y = -100000.0;
   p_min_x = 100000.0;
   p_min_y = 100000.0;
   
   p_prime_max_x = -100000.0;
   p_prime_max_y = -100000.0;
   p_prime_min_x = 1000000.0;
   p_prime_min_y = 1000000.0;
   
}


manual_stitch::~manual_stitch()
{
   
}

//////////////// Manual Stitcher Private Methods /////////////////////

void manual_stitch::determine_bounding_boxes()
{
   p_max_x = -100000.0;
   p_max_y = -100000.0;
   p_min_x = 100000.0;
   p_min_y = 100000.0;
   
   p_prime_max_x = -100000.0;
   p_prime_max_y = -100000.0;
   p_prime_min_x = 100000.0;
   p_prime_min_y = 100000.0;
   
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      //Determine p points extrema
      if( chosen_points[ curr_point ].x > p_max_x )
      {
         p_max_x = chosen_points[ curr_point ].x;
      }
      if( chosen_points[ curr_point ].x < p_min_x )
      {
         p_min_x = chosen_points[ curr_point ].x;
      }
      if( chosen_points[ curr_point ].y > p_max_y )
      {
         p_max_y = chosen_points[ curr_point ].y;
      }
      if( chosen_points[ curr_point ].y < p_min_y )
      {
         p_min_y = chosen_points[ curr_point ].y;
      }
      
      //Determine p_prime points extrema
      if( chosen_points[ curr_point + num_points_div_2 ].x > p_prime_max_x )
      {
         p_prime_max_x = chosen_points[ curr_point + num_points_div_2 ].x;
      }
      if( chosen_points[ curr_point + num_points_div_2 ].x < p_prime_min_x )
      {
         p_prime_min_x = chosen_points[ curr_point + num_points_div_2 ].x;
      }
      if( chosen_points[ curr_point + num_points_div_2 ].y > p_prime_max_y )
      {
         p_prime_max_y = chosen_points[ curr_point + num_points_div_2 ].y;
      }
      if( chosen_points[ curr_point + num_points_div_2 ].y < p_prime_min_y )
      {
         p_prime_min_y = chosen_points[ curr_point + num_points_div_2 ].y;
      }
   }
   
   //Pick the intermediate box dimensions to be the largest extrema
   //from the rough bounding boxes
   if( abs(p_max_x - p_min_x) > abs(p_prime_max_x - p_prime_min_x) )
   {
      box_x = abs(p_max_x - p_min_x);
   }
   else
   {
      box_x = abs(p_prime_max_x - p_prime_min_x);
   }
   
   if( abs(p_max_y - p_min_y) > abs(p_prime_max_y - p_prime_min_y) )
   {
      box_y = abs(p_max_y - p_min_y);
   }
   else
   {
      box_y = abs(p_prime_max_y - p_prime_min_y);
   }
   
   //Create the vector of box points which will act as "p points" in
   //both transforms -- from p to box and box to p prime. (is actually
   //box_to_p and box_to_p_prime
   box_points.push_back( Vec3d( 0.0, 0.0, 1.0 ) );
   box_points.push_back( Vec3d( 0.0, box_y-1, 1.0 ) );
   box_points.push_back( Vec3d( box_x-1, box_y-1, 1.0 ) );
   box_points.push_back( Vec3d( box_x-1, 0.0, 1.0 ) ); 
   
   intermediate_box.create( box_y, box_x, CV_8UC3 );
}
////////////////////////////////
// Author: Brian Fehrman
// Stitches one image into a frame contained within
// a second image.
/////////////////////////////

void manual_stitch::frame_stitch()
{
   determine_bounding_boxes();
   
   compute_H( box_points, chosen_p_points, box_to_p_H_matrix );
   compute_H( box_points, chosen_p_prime_points, box_to_p_prime_H_matrix );
   
   transform_p_to_box();
   transform_box_to_image();
   
   namedWindow("img1", WINDOW_SIZE_CHOICE );
   for(;;)
   {
      imshow("img1", p_prime_image);
         
      if( (int) waitKey( 30 ) > 0 ) break;
   }
}

//////////////////////////////
// Author: Brian Fehrman
// Gets points used for frame or
// mosaic manual/ semi-auto stitching
/////////////////////////////

void manual_stitch::get_points()
{
   Scalar colors[4] = {Scalar(0,220,220), Scalar(220,0,220), Scalar(220,220,0), Scalar(50,220,50) }; 
   Mat combined_images( image_y_max, image_x_max * 2, CV_8UC3 );
   Mat combined_images_highlight( image_y_max, image_x_max * 2, CV_8UC3 );
   stringstream converter;
   int fontFace = FONT_HERSHEY_DUPLEX;
   int key_pressed = 'a';
   
   for(int curr_row = 0; curr_row < p_image.rows; curr_row++ )
   {
      for(int curr_col = 0; curr_col < p_image.cols; curr_col++)
      {
         combined_images.at<Vec3b>( curr_row, curr_col ) = p_image.at<Vec3b>( curr_row, curr_col );
      }
   }
   
   for(int curr_row = 0; curr_row < p_prime_image.rows; curr_row++ )
   {
      for(int curr_col = 0; curr_col < p_prime_image.cols; curr_col++)
      {
         combined_images.at<Vec3b>( curr_row, curr_col + p_image.cols) = p_prime_image.at<Vec3b>( curr_row, curr_col );
      }
   }
   
   namedWindow("Pick corresponding points", WINDOW_SIZE_CHOICE );
   cvSetMouseCallback( "Pick corresponding points", mouse_callback, this );
   
   for(;;)
   {
      combined_images.copyTo( combined_images_highlight );
      
      for(unsigned int i = 0; i < chosen_points.size(); i++ )
      {

         circle(combined_images_highlight, chosen_points[ i ], 10, colors[i % 4], -1);
         converter << i % 4;
         putText (combined_images_highlight,converter.str(),chosen_points[ i ], fontFace, 2, colors[i % 4], 3);
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
   
   //If manual stitch, then 8 points are needed to be selected with 4 from
   //p image and 4 from p prime image. Otherwise, just 4 points from the
   //p prime image should be selected.
   if( stitch_type == MANUAL_FRAME_STITCH || stitch_type == MANUAL_MOSAIC_STITCH )
   {
      //Account for combined image offset of points
      for(unsigned int curr_p_prime_point = num_points_get / 2; curr_p_prime_point < num_points_get; curr_p_prime_point++ )
      {
         chosen_points[ curr_p_prime_point ].x -= p_image.cols;
      }
   }
   else
   {
      //Puts the points in the correct location of the chosen points array.
      //All of this is a bit hacked but oh well. Not getting paid to do it
      //and probably trying way too hard anyways.
      for( unsigned int curr_point = 0; curr_point < num_points_get; curr_point++ )
      {
         chosen_points.push_back( chosen_points[ curr_point ] );
         chosen_points[ num_points_get / 2 + curr_point ].x -= p_image.cols;
      }
   
      chosen_points[ 0 ] = Point( 0, 0 );
      chosen_points[ 1 ] = Point( p_image.cols - 1, 0 );
      chosen_points[ 2 ] = Point( p_image.cols - 1, p_image.rows - 1 );
      chosen_points[ 3 ] = Point( 0, p_image.rows - 1 );
   }  
}

void manual_stitch::manual_mosaic_stitch()
{
   Mat mosaic_image;
   Mat H_inv;
   
   compute_H( chosen_p_points, chosen_p_prime_points, H_matrix );
         
   create_mosaic( mosaic_image );
   
   namedWindow("img1", WINDOW_SIZE_CHOICE );
   for(;;)
   {
      imshow("img1", mosaic_image);
         
      if( (int) waitKey( 30 ) > 0 ) break;
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

void manual_stitch::mouse_callback( int event, int x, int y, int flags, void* param )
{
   //Static cast is because this is a static method. Just how c++ wanted it...
   manual_stitch* man_stitch_obj = static_cast<manual_stitch*>(param);
   
   switch( event )
   {
      //Select a point
      case CV_EVENT_LBUTTONDOWN:
        cout << " x: " << x << " y: " << y << endl;
        
        if( man_stitch_obj->chosen_points.size() < man_stitch_obj->num_points_get )
        {
           man_stitch_obj->chosen_points.push_back(Point( x, y ) );
        }
        break;
      
      //Deselect a point
      case CV_EVENT_RBUTTONDOWN:
        if( man_stitch_obj->chosen_points.size() > 0 )
        {
           man_stitch_obj->chosen_points.erase( man_stitch_obj->chosen_points.end() );
        }
        break;

      //Perform the homography
      case CV_EVENT_MBUTTONDOWN:
        cout << "Perform homography" << endl;
        break;
   }
}

int manual_stitch::run()
{   
   get_points();
   
   //Move points from single vector into seperate vectors for easier processing
   for( unsigned int curr_point = 0; curr_point < num_points_div_2; curr_point++ )
   {
      chosen_p_points.push_back( Vec3d( (double) chosen_points[ curr_point ].x, (double) chosen_points[ curr_point ].y, 1.0 ) );
   }
   
   for( unsigned int curr_point = num_points_div_2; curr_point < num_points; curr_point++ )
   {
      chosen_p_prime_points.push_back( Vec3d( (double) chosen_points[ curr_point ].x, (double) chosen_points[ curr_point ].y, 1.0 ) );
   }
   
   if( stitch_type == MANUAL_FRAME_STITCH || stitch_type == SEMI_AUTO_FRAME_STITCH )
   {
      frame_stitch();
   }
   else
   {
      manual_mosaic_stitch();
   }
   return 0;
}

///////////////////////////
// Author: Brian Fehrman
// Transforms the intermediate box points in to the new
// image frame.
//////////////////////////
void manual_stitch::transform_box_to_image()
{
   double scale_val = 1.0;
   double curr_tran_x = 1.0;
   double curr_tran_y = 1.0;

   for(double curr_x = 0; curr_x < box_x; curr_x += 1 )
   {   
      for( double curr_y = 0; curr_y < box_y; curr_y += 1 )
      {
         scale_val = curr_x * box_to_p_prime_H_matrix.at<double>(2,0)  + curr_y * box_to_p_prime_H_matrix.at<double>(2,1) + box_to_p_prime_H_matrix.at<double>(2,2);
         curr_tran_x = (curr_x * box_to_p_prime_H_matrix.at<double>(0,0) + curr_y * box_to_p_prime_H_matrix.at<double>(0,1) + box_to_p_prime_H_matrix.at<double>(0,2))/scale_val;
         curr_tran_y = (curr_x * box_to_p_prime_H_matrix.at<double>(1,0) + curr_y * box_to_p_prime_H_matrix.at<double>(1,1) + box_to_p_prime_H_matrix.at<double>(1,2))/scale_val;
         
         if(curr_tran_x > p_prime_image.cols)
         {
            curr_tran_x = p_prime_image.cols - 1;
         }
         if(curr_tran_x < 0)
         {
            curr_tran_x = 0;
         }
         if(curr_tran_y > p_prime_image.rows)
         {
            curr_tran_y = p_prime_image.rows- 1;
         }
         if(curr_tran_y < 0)
         {
            curr_tran_y = 0;
         }
         
         //Gets rid of missing pieces...isn't the best way to do it and is missing error checking but works for now.
         p_prime_image.at<Vec3b>( curr_tran_y-1, curr_tran_x-1 ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y-1, curr_tran_x ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y-1, curr_tran_x+1 ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y, curr_tran_x-1 ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y, curr_tran_x ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y, curr_tran_x+1 ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y+1, curr_tran_x-1 ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y+1, curr_tran_x ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         p_prime_image.at<Vec3b>( curr_tran_y+1, curr_tran_x+1 ) = intermediate_box.at<Vec3b>( curr_y, curr_x );
         
      }
   }
}

////////////////////
// Author: Brian Fehrman
// Takes the p points and makes them in to an intermediate box
// with the idea being it will be easier to trasnform between
// two arbitrary shapes.
///////////////////
void manual_stitch::transform_p_to_box()
{
   double scale_val = 1.0;
   int curr_tran_x = 1.0;
   int curr_tran_y = 1.0;

   for(double curr_x = 0; curr_x < box_x; curr_x += 1 )
   {   
      for( double curr_y = 0; curr_y < box_y; curr_y += 1 )
      {
         scale_val = curr_x * box_to_p_H_matrix.at<double>(2,0)  + curr_y * box_to_p_H_matrix.at<double>(2,1) + box_to_p_H_matrix.at<double>(2,2);
         curr_tran_x = (curr_x * box_to_p_H_matrix.at<double>(0,0) + curr_y * box_to_p_H_matrix.at<double>(0,1) + box_to_p_H_matrix.at<double>(0,2))/scale_val;
         curr_tran_y = (curr_x * box_to_p_H_matrix.at<double>(1,0) + curr_y * box_to_p_H_matrix.at<double>(1,1) + box_to_p_H_matrix.at<double>(1,2))/scale_val;
         
         if(curr_tran_x > p_image.cols)
         {
            curr_tran_x = p_image.cols - 1;
         }
         if(curr_tran_x < 0)
         {
            curr_tran_x = 0;
         }
         if(curr_tran_y > p_image.rows)
         {
            curr_tran_y = p_image.rows- 1;
         }
         if(curr_tran_y > p_image.rows)
         {
            curr_tran_y = 0;
         }
         
         intermediate_box.at<Vec3b>( curr_y, curr_x ) = p_image.at<Vec3b>( curr_tran_y, curr_tran_x );
      }
   }
}

