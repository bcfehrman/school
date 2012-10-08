#ifndef _VISIONFUNCTIONS_
#define _VISIONFUNCTIONS_

#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

void createGausianKernal( Mat &kernDst, double standardDeviation, bool fftKern = false);
Mat gaussianFilter( Mat &origImage, Mat &kernel );

#endif
