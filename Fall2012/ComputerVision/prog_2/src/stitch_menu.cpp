/*
 * stitch_menu.cpp
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 * 
 */


#include "stitch_menu.h"


stitch_menu::stitch_menu()
{
   build_menu();
}


stitch_menu::~stitch_menu()
{
   
}

////////////////////////////////
// Author: Brian Fehrman
// Builds the primitive display menu that is used for this program.
// Kind of like the idea of just a basic menu without a bunch of
// fancy GUI overhead.
///////////////////////////////
void stitch_menu::build_menu()
{
   //Scalar colors[] = {Scalar(0,220,220), Scalar(220,0,220), Scalar(220,220,0), Scalar(50,220,50), Scalar(150, 50, 150) }; 
   vector<Scalar> colors;
   int fontFace = FONT_HERSHEY_DUPLEX;
   unsigned int num_colors  =5;
 
   srand((unsigned)time(0));
   
   user_menu.create(800, 800, CV_8UC3);
   
   //Using random colors right now but could change
   for( unsigned int curr_color = 0; curr_color < num_colors; curr_color++ )
   {
      colors.push_back( Scalar( rand() % 255, rand() % 255, rand() % 255 ) );
   }
   
   //Here is where the menu items can be changed as needed
   putText (user_menu,"Please select an option for stitching",Point(100,50), fontFace, 1, colors[0], 2);
   putText (user_menu,"a) Auto Mosaic Stitch",Point(50,200), fontFace, 1, colors[1], 2);
   putText (user_menu,"f) Manual Frame Stitch",Point(50,300), fontFace, 1, colors[2], 2);
   putText (user_menu,"m) Manual Mosaic Stitch",Point(50,400), fontFace, 1, colors[3], 2);
   putText (user_menu,"s) Semi-Auto Frame Stitch",Point(50,500), fontFace, 1, colors[4], 2);
}

//////////////////////////
// Author: Brian Fehrman
// Handles getting the user choice for the flavor
// of stitching.
///////////////////////
char stitch_menu::get_menu_choice()
{
   char user_choice = 0;
   
   namedWindow("User Menu", WINDOW_SIZE_CHOICE);
   
   for(;;)
   {
      imshow("User Menu", user_menu);
      
      user_choice = waitKey( 30 );
      
      //Only exit the loop on a valid input
      switch( user_choice )
      {
         case 'a':
         case 'f':
         case 'm':
         case 's':
               return user_choice;
               break;   //Not needed but good practice
      }
   }
   
   return -1;
}
