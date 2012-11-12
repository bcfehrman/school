///////////////////
// Author: Brian Fehrman
// Include file for Computer Vision Fall 2012 program 2
///////////////////

#include "stitcher.h"
#include "manual_stitch.h"
#include <ctime>
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <opencv2/features2d/features2d.hpp>

/**** Defines ****/
#define WINDOW_SIZE_CHOICE CV_WINDOW_NORMAL

/**** Using Statements ****/
using namespace cv;
using namespace std;

