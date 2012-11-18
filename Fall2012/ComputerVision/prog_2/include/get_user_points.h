/*
 * get_user_points.h
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 * 
 */


#ifndef GET_USER_POINTS_H
#define GET_USER_POINTS_H

class get_user_points
{
   public:
      get_user_points();
      virtual ~get_user_points();
      void get_points(Mat& image, vector<Vec3d>& chosen_points, unsigned int num_points = 4);
      static void mouse_callback( int event, int x, int y, int flags, void* param );
   
   private:
      unsigned int num_points;
      vector<Vec3d>* chosen_points_ptr;
      /* add your private declarations */
};

#endif /* GET_USER_POINTS_H */ 
