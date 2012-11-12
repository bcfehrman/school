/*
 * manual_stitch.h
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 */


#ifndef MANUAL_STITCH_H
#define MANUAL_STITCH_H

#include <stitcher.h>

class manual_stitch: public stitcher
{
   public:
      manual_stitch();
      manual_stitch(string file_name_1, string file_name_2, const unsigned int num_points = 8);
      virtual ~manual_stitch();
   
   private:
      /* add your private declarations */
};

#endif /* MANUAL_STITCH_H */ 
