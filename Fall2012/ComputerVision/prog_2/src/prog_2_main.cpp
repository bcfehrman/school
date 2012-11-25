
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
   manual_stitch *man_stitch_obj;
   auto_stitch* auto_stitch_obj;
   char menu_choice = 'a';
   stitch_menu stitch_menu_obj;
   string p_image_file_name = "img/mine/buzzard1.jpg";
   string p_prime_image_file_name = "img/mine/buzzard2.jpg";
   
   menu_choice = stitch_menu_obj.get_menu_choice();
   
   switch( menu_choice )
   {
      case 'a':
         auto_stitch_obj = new auto_stitch( p_image_file_name, p_prime_image_file_name, 8, 3);
         auto_stitch_obj->run();
         break;
         
      case 'f':
         man_stitch_obj = new manual_stitch( p_image_file_name, p_prime_image_file_name, 8, 0);
         man_stitch_obj->run();
         break;
         
      case 'm':
         man_stitch_obj = new manual_stitch( p_prime_image_file_name, p_image_file_name, 8, 2);
         man_stitch_obj->run();
         break;
         
      case 's':
         man_stitch_obj = new manual_stitch( p_image_file_name, p_prime_image_file_name, 8, 1);
         man_stitch_obj->run();      
         break;
         
      default:
         cout << "Error getting menu choice, exiting ...";
            return -1;
   }
   
   delete man_stitch_obj;

   return 0;
}
