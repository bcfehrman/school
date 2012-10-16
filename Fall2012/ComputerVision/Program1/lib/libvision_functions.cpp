#include "libvision_functions.h"

//Generates a matrix of the autocorrelation value found at each point.
//Algorithm taken from Szeliski page 188.
void createAutoCorrMatrix( Mat& srcMat, Mat& dstMat, Mat& xDeriv, Mat& yDeriv, const float thresholdVal)
{
   int numRows = srcMat.size().height;
   int numCols = srcMat.size().width;
   float currIx, currIy, currIxIy;
   float traceVal, detVal, intensityVal;
   featVal featValStruct;
   
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
         intensityVal = detVal / traceVal;
         
         //Suppress values less than threshold
         if( intensityVal > thresholdVal )
         {
            dstMat.at<float>( i, j ) = intensityVal;
         }
         else
         {
            dstMat.at<float>( i, j ) = 0;
         }
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
void createDerivGaussianKernels( Mat& kernXDst, Mat& kernYDst, double standardDeviation)
{
	int width = kernXDst.size().width;
   int height = kernXDst.size().height;
   int widthDiv2 = kernXDst.size().width / 2;
   int heightDiv2 = kernXDst.size().height / 2;
	double variance = standardDeviation * standardDeviation;
	double num;
	double den = -sqrt(2 * M_PI) * standardDeviation * variance;
	int x,y,ySqrd;
	double kernelVal = 0;
   double preNormSumX = 0;
	double preNormSumY = 0;
   float* kernXPtrHead = (float*) kernXDst.data;
   float* kernXPtr = kernXPtrHead;
   float* kernYPtrHead = (float*) kernYDst.data;
   float* kernYPtr = kernYPtrHead;

   //Generate the gaussian kernel
	for(int i = 0; i < height; i++)
	{
		y = i -heightDiv2;
		ySqrd = y * y;
		
      for(int j = 0; j < width; j++)
      {
         x = j - widthDiv2;
      
         num =  exp(-1 * ( ( x * x + ySqrd) / ( 2 * variance ) ) );
         
         kernelVal = num / den;
         
         *kernXPtr = kernelVal * x;
         preNormSumX += *kernXPtr;
         kernXPtr++;
         
         *kernYPtr = kernelVal * y;
         preNormSumY += *kernYPtr;
         kernYPtr++;
      }
   }
   
   //Reset the pointer
   kernXPtr = kernXPtrHead;
   kernXPtr = kernYPtrHead;
   
   //Normalize the values
   for(int i = 0; i < height; i++)
   {
      for( int j = 0; j < width; j++ )
      {
         //*kernXPtr /= preNormSumX;
         kernXPtr++;
         
         //*kernYPtr /= preNormSumY;
         kernYPtr++;
      }
   }
}

//Creates a gaussian kernel which will be the size of the
//destination matrix
void createGaussianKernel( Mat& kernDst, double standardDeviation)
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

//Creates a gaussian kernel which will be the size of the
//destination matrix
void createNormLOGKernel( Mat& kernDst, double standardDeviation)
{
	int width = kernDst.size().width;
   int height = kernDst.size().height;
   int widthDiv2 = kernDst.size().width / 2;
   int heightDiv2 = kernDst.size().height / 2;
	double variance = standardDeviation * standardDeviation;
	int xSqrd,ySqrd;
	double kernelVal = -2 * variance;
	double preNormSum = 0;
	double normSum = 0;
   float* kernPtrHead = (float*) kernDst.data;
   float* kernPtr = kernPtrHead;

   //Generate the scale normalized Laplacian of Gaussian kernel
	for(int i = 0; i < height; i++)
	{
		ySqrd = i -heightDiv2;
		ySqrd *= ySqrd;
      kernelVal += ySqrd;
		
      for(int j = 0; j < width; j++)
      {
         xSqrd = j - widthDiv2;
         xSqrd *= xSqrd;
         kernelVal += xSqrd;
         kernelVal *=  exp(-1 * ( ( xSqrd + ySqrd) / ( 2 * variance ) ) );
         
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

void suppressNonMaximums( Mat& srcMat, Mat& dstMat, const int sizeNeighbor)
{
   int sizeNeighborDiv2 = sizeNeighbor / 2;
   float currCheck = 0.0;
   bool breakOut = false;
   int numRows = srcMat.rows;
   int numCols = srcMat.cols;
   int topRange = sizeNeighbor;
   int bottomRange = sizeNeighbor;
   int leftRange = sizeNeighbor;
   int rightRange = sizeNeighbor;
   
   for(int i = 1; i < numRows; i++)
   {
      if(i < sizeNeighbor )
      {
         topRange = i;
      }
      else if( i >= numRows - sizeNeighbor )
      {
         bottomRange = numRows - i;
      }
      
      rightRange = sizeNeighbor;
      leftRange = sizeNeighbor;
      
      for( int j = 0; j < numCols; j++ )
      {
         if(j < sizeNeighbor )
         {
            leftRange = j;
         }
         else if( j >= numCols - sizeNeighbor )
         {
            rightRange = numCols - j;
         }
         
         //Get the current pixel to check and set it to 90% of its
         //value to make sure that it is at least 10% greater in value
         //than its neighbors
         currCheck = srcMat.at<float>(i,j) * 0.9;
         
         //Skip over points that are 0
         if( currCheck != 0 )
         {
            //Check the neigbhorhood to both the right and the bottom of the
            //current pixel. No need to check left and up because this is taken
            //care of by the right/bottom checking
            for(int currRow = i - topRange; currRow < i + bottomRange; currRow++)
            {
               for( int currCol = j  - leftRange; currCol < j + rightRange; currCol++)
               {
                  //If neighboring pixel is less than current, then zero it
                  //Else set the current pixel to be zero and move on
                  
                  if( currRow == i && currCol == j )
                  {
                     break;
                  }
                  else if( currCheck >= srcMat.at<float>( currRow, currCol ) )
                  {
                     srcMat.at<float>( currRow, currCol ) = 0;
                     dstMat.at<float>( currRow, currCol ) = 0;
                  }
                  else
                  {
                     srcMat.at<float>( i, j ) = 0;
                     dstMat.at<float>( i, j) = 0;
                     breakOut = true;
                     break;
                  }
               }
               
               //Handles exiting the loops if current pixel was zeroed
               if( breakOut )
               {
                  breakOut = false;
                  break;
               }
               else
               {
                  dstMat.at<float>(i, j) = srcMat.at<float>( i, j );
               }
            }
         }
      }
   }
}
