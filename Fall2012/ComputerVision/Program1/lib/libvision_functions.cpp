#include "libvision_functions.h"

//Generates a matrix of the autocorrelation value found at each point.
//Algorithm taken from Szeliski page 188.
void createAutoCorrMatrix( Mat& srcMat, Mat& dstMat, Mat& xyDeriv, Mat& xDerivSqd, Mat& yDerivSqd, const float thresholdVal)
{
   int numRows = srcMat.size().height;
   int numCols = srcMat.size().width;
   float currIxSqd, currIySqd, currIxIy;
   float traceVal, detVal, intensityVal;
   featVal featValStruct;
   
   for( int i = 0; i < numRows; i++ )
   {
      for( int j = 0; j < numCols; j++ )
      {
         //So we don't have to unnecessarily index into the matrix
         currIxSqd = xDerivSqd.at<float>( i, j );
         currIySqd = yDerivSqd.at<float>( i, j );
         currIxIy = xyDeriv.at<float>(i, j); 
         
         //Calculate trace and determinant of the 2x2 matrix
         detVal = ( currIxSqd * currIySqd ) - ( currIxIy * currIxIy );
         traceVal = ( currIxSqd  + currIySqd);
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
   //instead of convolution although I'm still not convinced
   //that it matters for these matrices.
   flip(GX, GX, -1);
   flip(GY, GY, -1);
}

//Creates a gaussian kernel which will be the size of the
//destination matrix
void createDerivGaussianKernels( Mat& kernXDst, Mat& kernYDst, float standardDeviation)
{
	int width = kernXDst.size().width;
   int height = kernXDst.size().height;
   int widthDiv2 = kernXDst.size().width / 2;
   int heightDiv2 = kernXDst.size().height / 2;
	float variance = standardDeviation * standardDeviation;
	float num = 0;
	float den = -sqrt(2 * M_PI);
	int x = 0, y = 0, ySqrd = 0;
	float kernelVal = 0;
   float preNormSumX = 0;
	float preNormSumY = 0;
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

         if( x == 0 )
         {
            *kernXPtr = 0;
         }
         else
         {
            *kernXPtr = kernelVal * x;
         }
         
         kernXPtr++;
         
         if( y == 0 )
         {
            *kernYPtr = 0;
         }
         else
         {
            *kernYPtr = kernelVal * y;
         }
         kernYPtr++;
      }
   }
}

//Creates a gaussian kernel which will be the size of the
//destination matrix
void createGaussianKernel( Mat& kernDst, float standardDeviation)
{
	int width = kernDst.size().width;
   int height = kernDst.size().height;
   int widthDiv2 = kernDst.size().width / 2;
   int heightDiv2 = kernDst.size().height / 2;
	float variance = standardDeviation * standardDeviation;
	float num = 0;
	float den = 2 * M_PI;
	int x = 0, ySqrd = 0;
	float kernelVal = 0;
	float preNormSum = 0;
	float normSum = 0;
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
void createNormLOGKernel( Mat& kernDst, float standardDeviation)
{
	int width = kernDst.size().width;
   int height = kernDst.size().height;
   int widthDiv2 = kernDst.size().width / 2;
   int heightDiv2 = kernDst.size().height / 2;
	float variance = standardDeviation * standardDeviation;
	int xSqrd = 0, ySqrd = 0;
	float kernelVal = -2 * variance;
	float preNormSum = 0;
	float normSum = 0;
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

void findOrientations( Mat& srcMat, vector<featVal>& featVec, Mat srcXKern[NUM_SCALES], Mat srcYKern[NUM_SCALES] )
{
   Mat tmp;
   float currScale = 0.0;
   
   for(int i = 0; i < featVec.size(); i++ )
   {
      currScale = featVec.at(i).scale;
   }
}

//Uses the Laplacian of Gaussian to find the scale at which the current
//feature is an extrema and sets this as the feature's characteristic
//scale
void findScales( Mat& srcMat, vector<featVal>& featVec, Mat LOGKernels[NUM_SCALES], const float startSTD )
{
   float maxResponse = 0.0;
   float currScale = startSTD;
   float maxScale = 0.0;
   float currResponse = 0.0;
   
   for( int i = 0; i < featVec.size(); i++ )
   {
      currScale = startSTD;
      maxResponse = 0.0;
        
      for(int j = 0; j < NUM_SCALES; j++ )
      {               
         //Get the Laplacian of Gaussian measurement for the current feature point
         currResponse = scalerConvolveMatrixRegion( srcMat, LOGKernels[j], featVec.at(i).iPos, featVec.at(i).jPos );
         
         //Want either min or max
         currResponse *= currResponse;
         
         if( currResponse > maxResponse )
         {
            maxResponse = currResponse;
            maxScale = currScale;
         }
         
         currScale += currScale;
      }
      
      featVec.at(i).scale = maxScale;
   }
}

//Puts the center of the kernel on the matConvCenter point in srcMat and then
//performs convolution at the point and returns the result
float scalerConvolveMatrixRegion( Mat& srcMat, Mat& kernel, const int iPos, const int jPos)
{
   float result = 0.0;
   int srcMatCols = srcMat.cols;
   int srcMatRows = srcMat.rows;
   int kernelSize = kernel.rows;
   int kernelSizeDiv2 = kernelSize / 2;
   int curr_i_pos = iPos - kernelSizeDiv2;
   int curr_j_pos = 0;
   
   for( int i = 0; i < kernelSize; i++, curr_i_pos++ )
   {
      if( curr_i_pos < 0 )
         continue;
      else if( curr_i_pos >= srcMatRows )
         break;
         
      curr_j_pos = jPos - kernelSizeDiv2;
      
      for( int j = 0; j < kernelSize; j++, curr_j_pos++ )
      {
               
         if( curr_j_pos < 0 )
            continue;
         else if( curr_j_pos >= srcMatCols )
            break;

         result += ( kernel.at<float>( i, j ) * srcMat.at<float>( curr_i_pos, curr_j_pos ) ) ;
      }
   }
   
   return result;
}

//Sort function used for sorting a vector of featVal structs
//and is based upon their intensity response value wrt the
//harrison operator.
bool sortFeatVal( featVal featVal1, featVal featVal2 )
{
   return ( featVal1.intensityVal < featVal2.intensityVal );
}

void suppressNonMaximumsAdaptive( Mat& srcMat, vector<featVal>& dstVec, const int sizeNeighbor, const int numKeep)
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
   featVal currFeat;
   
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
            breakOut = false;
            
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
                  }
                  else
                  {
                     srcMat.at<float>( i, j ) = 0;
                     breakOut = true;
                     break;
                  }
               }
               
               //Handles exiting the loops if current pixel was zeroed
               if( breakOut )
               {
                  break;
               }
            }
            
            if(!breakOut)
            {
               currFeat.iPos = i;
               currFeat.jPos = j;
               currFeat.intensityVal = srcMat.at<float>( i, j );
               dstVec.push_back( currFeat );
            }
         }
      }
   }
   
   //Make sure we have features
   if( !dstVec.empty() )
   {
      //Sort based on harris response value
      sort( dstVec.begin(), dstVec.end(), sortFeatVal );
      
      //If more features than desired, get rid of the extra features.
      if( dstVec.size() > numKeep )
      {
         dstVec.erase( dstVec.begin() + numKeep, dstVec.end() );
      }
   }
}
