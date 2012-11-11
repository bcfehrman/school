/*
 * homography.h
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 */


#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

class homography
{
   public:
      homography();
      virtual ~homography();
   
   private:
         Mat A_mat(8,9, CV_32F), H_mat(1,9, CV_32F);
};

#endif /* HOMOGRAPHY_H */ 
