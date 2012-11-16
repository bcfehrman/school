
/**
Author: Brian Fehrman
Class: Computer Vision Fall 2012
Professor: Dr. Hoover
Assign: Program 2

**/

/***** Includes *****/
#include "prog_2.h"

/******** Main ***********/


int main( int argc, char *argv[])
{  
   SurfFeatureDetector sfd;
   manual_stitch *man_stitch_obj = new manual_stitch( "img/mine/my_face_1.jpg", "img/graf/img4.ppm", 8);
   
   man_stitch_obj->run();
   
   delete man_stitch_obj;

   return 0;
}
