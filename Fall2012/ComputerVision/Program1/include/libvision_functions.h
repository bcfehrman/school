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

void createAutoCorrMatrix( Mat& srcMat, Mat& dstMat, Mat& xDeriv, Mat& yDeriv, const float thresholdVal);
void createDeriveKernels( Mat& GX, Mat& GY);
void createDerivGaussianKernels( Mat& kernXDst, Mat& kernYDst, double standardDeviation);
void createGaussianKernal( Mat& kernDst, double standardDeviation);
void suppressNonMaximums( Mat& srcMat, Mat& dstMat, const int sizeNeighbor);

#endif
