/*
 * stitch_menu.h
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>

 */


#ifndef STITCH_MENU_H
#define STITCH_MENU_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctime>

using namespace cv;

#define WINDOW_SIZE_CHOICE CV_WINDOW_NORMAL

class stitch_menu
{
   public:
      stitch_menu();
      virtual ~stitch_menu();
      char get_menu_choice();
   
   private:
   
   //Private Members
      Mat user_menu;
      
   //Private Methods
      void build_menu();
};

#endif /* STITCH_MENU_H */ 
