
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
   manual_stitch *man_stitch_obj = new manual_stitch( "img/graf/img1.ppm", "img/graf/img2.ppm", 8);
   
   man_stitch_obj->run();
   
   /*
   //Perform the SVD that gives the U, W, Vt (or U, S, Vt) except W is just a vector of the singular values
   SVD svd(im1_gray);

   cout << svd.u.rows << " " << svd.u.cols << endl;

   cout << svd.w.rows << " " << svd.w.cols << endl;

   cout << svd.vt.rows << " " << svd.vt.cols << endl;
   */

   return 0;
}
