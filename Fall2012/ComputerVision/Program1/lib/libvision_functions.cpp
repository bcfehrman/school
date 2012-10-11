#include "libvision_functions.h"


//Generates a matrix of the autocorrelation value found at each point.
//Algorithm taken from Szeliski page 188.
void createAutoCorrMatrix( Mat& srcMat, Mat& dstMat, Mat& xDeriv, Mat& yDeriv)
{
   int numRows = srcMat.size().height;
   int numCols = srcMat.size().width;
   float currIx, currIy, currIxIy;
   float traceVal, detVal;
   
   for( int i = 0; i < numRows; i++ )
   {
      for( int j = 0; j < numCols; j++ )
      {
         //So we don't have to unnecessarily index into the matrix
         currIx = xDeriv.at<float>( i, j );
         currIy = yDeriv.at<float>( i, j );
         currIxIy = currIx * currIy; 
         
         //Calculate trace and determinant of the 2x2 matrix
         detVal = ( ( currIx * currIx ) * ( currIy * currIy ) ) - ( currIxIy * currIxIy );
         traceVal = ( ( currIx * currIx ) + ( currIy * currIy ) );
         
         dstMat.at<float>( i, j ) = detVal / traceVal;
      } 
   }
}

//Creates 3x3 Sobel operator kernels that
//are used to approximate the x and y
//derivatives of a matrix
void createDeriveKernels( Mat& GX, Mat& GY)
{
   int value1 = -1;
   int value2 = -2;
   
   GX.create(3, 3, CV_32F);
   GY.create(3, 3, CV_32F);
   
   //This is just a slightly more compact
   //way to generate the matrices instead
   //of hardcoding each value
   //value1 and value2 seem non-descriptive
   //but they refer to the multiple of values that
   //appear (i.e., value1 = -1, 0, 1)
   for(int i = 0; i < 3; i++)
   {
      GX.at<float>(0, i) = value1;
      GX.at<float>(1, i) = value2;
      GX.at<float>(2, i) = value1;
      
      GY.at<float>(i, 0) = value1;
      GY.at<float>(i, 1) = value2;
      GY.at<float>(i, 2) = value1;
      
      value1 += 1;
      value2 += 2;
   }
   
   //Flip around both axes since opencv uses correlation
   //instead of convolution
   flip(GX, GX, -1);
   flip(GY, GY, -1);
}

//Creates a gaussian kernel which will be the size of the
//destination matrix
void createGaussianKernal( Mat& kernDst, double standardDeviation)
{
	int width = kernDst.size().width;
   int height = kernDst.size().height;
   int widthDiv2 = kernDst.size().width / 2;
   int heightDiv2 = kernDst.size().height / 2;
	double variance = standardDeviation * standardDeviation;
	double num;
	double den = 2 * M_PI * variance;
	int x,ySqrd;
	double kernelVal = 0;
	double preNormSum = 0;
	double normSum = 0;
   float* kernPtrHead = (float*) kernDst.data;
   float* kernPtr = kernPtrHead;

   //Generate the gaussian kernel
	for(int i = 0; i < height; i++)
	{
		ySqrd = i -heightDiv2;
		ySqrd *= ySqrd;
		
      for(int j = 0; j < width; j++)
      {
         x = j - widthDiv2;
      
         num =  exp(-1 * ( ( x * x + ySqrd) / ( 2 * variance ) ) );
         
         kernelVal = num / den;
         
         *kernPtr = kernelVal;
         kernPtr++;
         
         preNormSum += kernelVal;
      }
   }
   
   //Reset the pointer
   kernPtr = kernPtrHead;
   
   //Normalize the values
   for(int i = 0; i < height; i++)
   {
      for( int j = 0; j < width; j++ )
      {
         *kernPtr = *kernPtr / preNormSum;
         normSum += *kernPtr;
   
         kernPtr++;
      }
   }
}
