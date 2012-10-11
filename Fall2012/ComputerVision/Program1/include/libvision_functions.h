#ifndef _VISIONFUNCTIONS_
#define _VISIONFUNCTIONS_

#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

void createAutoCorrMatrix( Mat& srcMat, Mat& dstMat, Mat& xDeriv, Mat& yDeriv);
void createDeriveKernels( Mat& GX, Mat& GY);
void createGaussianKernal( Mat& kernDst, double standardDeviation);

#endif
