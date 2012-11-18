
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
   //SurfFeatureDetector sfd;
   manual_stitch *man_stitch_obj;
   char menu_choice = 'a';
   stitch_menu stitch_menu_obj;
   
   menu_choice = stitch_menu_obj.get_menu_choice();
   
   switch( menu_choice )
   {
      case 'a':

         break;
         
      case 'f':
         man_stitch_obj = new manual_stitch( "img/mine/my_face_1.jpg", "img/graf/img4.ppm", 8, 0);
         man_stitch_obj->run();
         break;
         
      case 'm':
      
         break;
         
      case 's':
         man_stitch_obj = new manual_stitch( "img/mine/my_face_1.jpg", "img/graf/img4.ppm", 8, 1);
         man_stitch_obj->run();      
         break;
         
      default:
         cout << "Error getting menu choice, exiting ...";
            return -1;
   }
   
   delete man_stitch_obj;

   return 0;
}
