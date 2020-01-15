#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/*!
  \brief Detects the eyes on an image and draw circles on it
*/
void detectAndDraw( Mat& img, double scale);
