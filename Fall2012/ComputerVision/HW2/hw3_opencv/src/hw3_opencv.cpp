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
	int height;

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

	//Get the image data
	height = im1_rgb.rows;
	//width = i;
	//step = img1->widthStep;
	//channels = img->nChannels;
	//data = (uchar *)img->imageData;
	//printf("Processing a %d%d image with %d channels \n", height, width;

	//Convert to 32bit
	im1_rgb.convertTo(im1_rgb, CV_32F, 1/255.0);
	
	namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	imshow("Display window", im1_rgb);

	cvtColor(im1_rgb, im1_gray, CV_RGB2GRAY);	

	namedWindow("Gray window", CV_WINDOW_AUTOSIZE);
	imshow("Gray window", im1_gray);

	SVD svd(im1_gray);

	cout << svd.u.rows << " " << svd.u.cols << endl;

	cout << svd.w.rows << " " << svd.w.cols << endl;

	cout << svd.vt.rows << " " << svd.vt.cols << endl;

	Mat imnew = svd.u.rowRange(0,5) * Mat::diag(svd.w).rowRange(0,5) *  svd.vt.rowRange(0,5);

	
	//multiply(svd.u, svd.w, imnew);
	//multiply(imnew, vt, imnew);
	namedWindow("Reconstruct", CV_WINDOW_AUTOSIZE);
	imshow("Reconstruct", imnew);

	waitKey(0);

	return 0;
}
