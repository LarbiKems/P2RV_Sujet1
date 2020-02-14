#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "windows.h" //j'etais obligé désopasdéso (il n'y a pas de multiplateforme pour l'appui touche)

void appuiTouche(int &decPixVert, int &decPixHor);
void calculDecalageFenetre(float positionTete[3], int &decalagePixelVertical, int &decalagePixelHorizontal, float distanceCameraPaysage, float pixelParMetre);
bool decoupageImage(const cv::Range decoupageLigne, const cv::Range decoupageColonne, const  int decalagePixelHorizontal, const int decalagePixelVertical,cv::Range &decoupageLigne2, cv::Range &decoupageColonne2 , cv::Mat curImg);