//is HW3 with the SVD but will try to use video capture

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char *argv[])
{	
	Mat frame_gray;
	Mat w, u, vt;
        VideoCapture cap(0); //open the default camera
	Mat imnew;

	if(!cap.isOpened()) //check if we succeeded
		return -1;


	namedWindow("Display window", CV_WINDOW_AUTOSIZE);

	for(;;)
	{
		Mat frame_bgr;
		
		cap >> frame_bgr;

		//Convert to 32bit float
		frame_bgr.convertTo(frame_bgr, CV_32F, 1/255.0);
		//Convert to gray scale
		cvtColor(frame_bgr, frame_gray, CV_BGR2GRAY);	

		//Perform the SVD that gives the U, W, Vt (or U, S, Vt) except W is just a vector of the singular values
		SVD svd(frame_gray);

		//Reconstruct the iamge, the W vector is converted into a diagonal matrix so that it plays nicely, might be able to make that better
		//imnew = svd.u(Range::all(), Range(0,5)) * Mat::diag(svd.w)(Range(0,5),Range(0,5)) *  svd.vt(Range(0,5), Range::all());

		//namedWindow("Gray window", CV_WINDOW_AUTOSIZE);
		imshow("Display window", frame_gray);

		if(waitKey(30) >= 0) break;
	}

	return 0;
}
