#ifndef _VISIONFUNCTIONS_
#define _VISIONFUNCTIONS_

#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

void createGausianKernal( Mat &kernDst, double standardDeviation, const bool fftKern = false);
void gaussianFilter( Mat &origImage, Mat &kernel, Mat &destImage );

#endif
