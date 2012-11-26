
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
   string output_mosaic_name = "output.jpg";
   string p_image_file_name = "";
   string p_prime_image_file_name = "";
   
   if( argc < 3 )
   {
      cout << "Usage: progname p_image_name(left) p_prime_image_name(right) [output_mosaic_name= output.jpg]" << endl;
      return -1;
   }
   if(argc == 4 )
   {
      output_mosaic_name = argv[ 3 ];
   }
   
   p_image_file_name = argv[ 1 ];
   p_prime_image_file_name = argv[ 2 ];
   
   menu_choice = stitch_menu_obj.get_menu_choice();
   
   switch( menu_choice )
   {
      case 'a':
         auto_stitch_obj = new auto_stitch( p_image_file_name, p_prime_image_file_name, 8, output_mosaic_name, AUTO_MOSAIC_STITCH);
         auto_stitch_obj->run();
         break;
         
      case 'f':
         man_stitch_obj = new manual_stitch( p_image_file_name, p_prime_image_file_name, 4, output_mosaic_name, MANUAL_FRAME_STITCH);
         man_stitch_obj->run();
         break;
         
      case 'm':
         man_stitch_obj = new manual_stitch( p_prime_image_file_name, p_image_file_name, 4, output_mosaic_name, MANUAL_MOSAIC_STITCH);
         man_stitch_obj->run();
         break;
         
      case 's':
         man_stitch_obj = new manual_stitch( p_image_file_name, p_prime_image_file_name, 4, output_mosaic_name, SEMI_AUTO_FRAME_STITCH);
         man_stitch_obj->run();      
         break;
         
      default:
         cout << "Error getting menu choice, exiting ...";
            return -1;
   }
   
   delete man_stitch_obj;

   return 0;
}
