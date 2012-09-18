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
	Mat im1_rgb;
	Mat im1_gray;
	Mat w, u, vt;

	if(argc < 2)
	{
		printf("Usage: main <image-file-name>\n\7");
		exit(0);
	}

	//Load the first image
	im1_rgb = imread(argv[1]);
	if(!im1_rgb.data)
	{
		printf("Could not load image file: %s\n", argv[1]);
		exit(0);
	}

	//Convert to 32bit
	im1_rgb.convertTo(im1_rgb, CV_32F, 1/255.0);
	
	namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	imshow("Display window", im1_rgb);

	//Convert to gray scale
	cvtColor(im1_rgb, im1_gray, CV_RGB2GRAY);	

	namedWindow("Gray window", CV_WINDOW_AUTOSIZE);
	imshow("Gray window", im1_gray);

	//Perform the SVD that gives the U, W, Vt (or U, S, Vt) except W is just a vector of the singular values
	SVD svd(im1_gray);

	cout << svd.u.rows << " " << svd.u.cols << endl;

	cout << svd.w.rows << " " << svd.w.cols << endl;

	cout << svd.vt.rows << " " << svd.vt.cols << endl;

	//Reconstruct the iamge, the W vector is converted into a diagonal matrix so that it plays nicely, might be able to make that better
	Mat imnew = svd.u(Range::all(), Range(0,5)) * Mat::diag(svd.w)(Range(0,5),Range(0,5)) *  svd.vt(Range(0,5), Range::all());

	cout << imnew.rows << " " << imnew.cols << endl;	

	namedWindow("Reconstruct", CV_WINDOW_NORMAL);
	imshow("Reconstruct", imnew);

	waitKey(0);

	return 0;
}
