/*
 * auto_stitch.h
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 * 
 */


#ifndef AUTO_STITCH_H
#define AUTO_STITCH_H

#include <stitcher.h>

#define WINDOW_SIZE_CHOICE CV_WINDOW_NORMAL
#define AUTO_MOSAIC_STITCH 3

class auto_stitch: public stitcher
{
   public:
      auto_stitch();
      auto_stitch(string p_file_name, string p_prime_file_name, const unsigned int num_points, const unsigned int stitch_type);
      virtual ~auto_stitch();
      int run();
      static bool sort_match_val( DMatch match_val_1, DMatch match_val_2 );
      static bool sort_response_val( KeyPoint respone_val_1, KeyPoint response_val_2 );
   
   private:
      void ransac_auto_stitch(const vector<KeyPoint>& p_prime_key_points, const vector<KeyPoint>& p_prime_suppressed_key_points,
                                       const vector<DMatch>& point_matches);
      void surf_suppress_non_maximums_adaptive( const Mat& src_mat, const vector<KeyPoint>& src_vec, 
                                                vector<KeyPoint>& dst_vec, const int size_neighbor, const unsigned int num_keep);
};

#endif /* AUTO_STITCH_H */ 
