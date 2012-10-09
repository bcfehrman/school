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

void fftEdgeDetect(Mat &frameSrc, Mat &frameDst, Mat &FFTKern, double cuttOff)
{
   dft(frameSrc, frameSrc);            // this way the result may fit in the source matrix

   for(int i = 0; i < FFTKern.size().height; i++)
   {
      for(int j = 0; j < FFTKern.size().width; j++)
      {
         frameSrc.at<float>(i,j) *= FFTKern.at<float>(i,j);
      }
   }

   idft(frameSrc, frameSrc);
   normalize(frameSrc, frameSrc, 0, 1, CV_MINMAX);
    
   for(int i = 0; i < FFTKern.size().height; i++)
   {
      for(int j = 0; j < FFTKern.size().width; j++)
      {
         if(frameSrc.at<float>(i,j) < cuttOff)
            frameSrc.at<float>(i,j) = 0;
         else
            frameSrc.at<float>(i,j) = 1;
      }
   }
   
   for(int i = 0; i < frameDst.size().height; i++)
   {
      for(int j = 0; j < frameDst.size().width; j++)
      {
         if(!frameSrc.at<float>(i,j))
         {
            for(int k = -1; k < 1; k++)
               for( int l = -1; l<1; l++)
               {
                  if(i + k > 0 && i + k < frameDst.size().height
                     && j + l > 0 && j + l < frameDst.size().width)
                     {
                        frameDst.at<Vec3b>(i+k,j+l).val[1] = 0;
                        frameDst.at<Vec3b>(i+k,j+l).val[0] = 100;
                        frameDst.at<Vec3b>(i+k,j+l).val[2] = 100;
                     }
               }
         }
      }
   }
}
