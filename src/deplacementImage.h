#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#ifdef _WIN32
#include "windows.h" //j'etais oblig� d�sopasd�so (il n'y a pas de multiplateforme pour l'appui touche)
#endif
void ChangementPointDeVue(cv::Mat img, cv::Mat projMatrix, cv::Point3f headPosition, cv::Mat result);