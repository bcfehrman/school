#include "vision_functions.h"


void createGausianKernal( Mat &kernDst, double standardDeviation, const bool fftKern = false)
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

	for(int i = 0; i < height; i++)
	{
		ySqrd = i -heightDiv2;
		ySqrd *= ySqrd;
		
      if( fftKern )
      {
         for(int j = 0; j < width; j++)
         {
            x = j - widthDiv2;
         
            num =  1 - exp(-1 * ( ( x * x + ySqrd) / ( 2 * variance ) ) );
            
            kernelVal = num / den;
            
            *kernPtr = kernelVal;
            kernPtr++;
            
            preNormSum += kernelVal;
         }
      }
      else
      {
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
	}
   
   if( fftKern )
   {
      Mat q0(kernDst, Rect(0, 0, widthDiv2, heightDiv2));   // Top-Left - Create a ROI per quadrant
      Mat q1(kernDst, Rect(widthDiv2, 0, widthDiv2, heightDiv2));  // Top-Right
      Mat q2(kernDst, Rect(0, heightDiv2, widthDiv2, heightDiv2));  // Bottom-Left
      Mat q3(kernDst, Rect(widthDiv2, heightDiv2, widthDiv2, heightDiv2)); // Bottom-Right

      Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
      q0.copyTo(tmp);
      q3.copyTo(q0);
      tmp.copyTo(q3);

      q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
      q2.copyTo(q1);
      tmp.copyTo(q2);
   }
   else
   {
      kernPtr = kernPtrHead;
      
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
}

void gaussianFilter( Mat &origImage, Mat &kernel, Mat &destImage )
{
	int origRowSize = origImage.size().height;
	int origColSize = origImage.size().width;
	int kernelSize = kernel.size().width;
	int kernelSizeDiv2 = kernelSize / 2;
	int newRowSize = origRowSize - (kernelSizeDiv2 * 2);
	int newColSize = origColSize - (kernelSizeDiv2 * 2);
	
	Mat filteredImage(newRowSize, newColSize, CV_32F);
	double average = 0;
	int currRow, currCol, newRow, newCol, filtRow, filtCol, kernelRow, kernelCol;

	for(currRow = 0, newRow = 0; currRow < newRowSize; currRow++, newRow++)
	{ 
		for(currCol = 0, newCol = 0; currCol < newColSize; currCol++, newCol++)
		{
			average = 0;
		
		for(filtRow = currRow, kernelRow = 0; filtRow < (currRow +  kernelSize); filtRow++, kernelRow++)
		{
			
			for( filtCol = currCol, kernelCol = 0; filtCol < (currCol +  kernelSize); filtCol++, kernelCol++)
			{
				average += origImage.at<float>(filtRow, filtCol) * kernel.at<float>(kernelRow, kernelCol);
			}
		}
			filteredImage.at<float>(newRow, newCol) = average;
		}
	}

	destImage = filteredImage;
}
