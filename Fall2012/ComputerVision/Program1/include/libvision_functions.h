#ifndef _VISIONFUNCTIONS_
#define _VISIONFUNCTIONS_

#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

struct featVal{
  int x;
  int y;
  double intensityVal;
  double orientation; 
};

void createAutoCorrMatrix( Mat& srcMat, Mat& dstMat, Mat& xDeriv, Mat& yDeriv);
void createDeriveKernels( Mat& GX, Mat& GY);
void createGaussianKernal( Mat& kernDst, double standardDeviation);

#endif
