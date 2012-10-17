#ifndef _VISIONFUNCTIONS_
#define _VISIONFUNCTIONS_

#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

struct featVal{
  Point pos;
  float intensityVal;
  float majorOrientation;
  float minorOrientations[10];
  float scale; 
};

void createAutoCorrMatrix( Mat& srcMat, Mat& dstMat, Mat& xDeriv, Mat& yDeriv, const float thresholdVal);
void createDeriveKernels( Mat& GX, Mat& GY);
void createDerivGaussianKernels( Mat& kernXDst, Mat& kernYDst, double standardDeviation);
void createGaussianKernel( Mat& kernDst, double standardDeviation);
void createNormLOGKernel( Mat& kernDst, double standardDeviation);
bool sortFeatVal( featVal featVal1, featVal featVal2 );
void suppressNonMaximumsAdaptive( Mat& srcMat, vector<featVal>& featVec, const int sizeNeighbor, const int numKeep);

#endif
