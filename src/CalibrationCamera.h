#define KEY_ESCAPE 27
#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "opencv2\opencv.hpp"
#include <iostream>
#include <string>
#define ESC_KEY 27

cv::Mat calibrateCamera(cv::VideoCapture cap);