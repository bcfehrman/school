/*
 * manual_stitch.h
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 */


#ifndef MANUAL_STITCH_H
#define MANUAL_STITCH_H

#include <stitcher.h>

#define WINDOW_SIZE_CHOICE CV_WINDOW_NORMAL
#define MANUAL_FRAME_STITCH 0
#define SEMI_AUTO_FRAME_STITCH 1
#define MANUAL_MOSAIC_STITCH 2

class manual_stitch: private stitcher
{
   public:
   
   //Public Members
      unsigned int stitch_type;
      unsigned int num_points_get;
   
   //Public Methods
      manual_stitch();
      manual_stitch(string p_file_name, string p_prime_file_name, const unsigned int num_points = 8, const unsigned int stitch_type = 0);
      virtual ~manual_stitch();
      static void mouse_callback( int event, int x, int y, int flags, void* param );
      int run();
   
   private:
   
   //Private Members
      double box_x, box_y;
      vector<Vec3d> box_points;
      Mat intermediate_box;
      vector<Point> chosen_points;
      vector<Vec3d> chosen_p_points, chosen_p_prime_points;
      Mat box_to_p_H_matrix, box_to_p_prime_H_matrix;
      double p_max_x, p_max_y;
      double p_min_x, p_min_y;
      double p_prime_max_x, p_prime_max_y;
      double p_prime_min_x, p_prime_min_y;
      
   //Private Methods
      void compute_h_matrices();
      void determine_bounding_boxes();
      void frame_stitch();
      void get_points();
      void manual_mosaic_stitch();
      void transform_box_to_image();
      void transform_p_to_box();
};

#endif /* MANUAL_STITCH_H */ 
