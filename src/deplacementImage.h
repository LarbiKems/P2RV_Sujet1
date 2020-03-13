#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#ifdef _WIN32
#include "windows.h" //j'etais oblig� d�sopasd�so (il n'y a pas de multiplateforme pour l'appui touche)
#endif
#ifdef BENJI_ET_SES_FONCTIONS_A_LA_CON
void appuiTouche(int &decPixVert, int &decPixHor);
#endif
void calculDecalageFenetre(cv::Point3f positionTete, int &decalagePixelVertical, int &decalagePixelHorizontal, float distanceCameraPaysage, float pixelParMetre);
bool decoupageImage(const cv::Range decoupageLigne, const cv::Range decoupageColonne, const  int decalagePixelHorizontal, const int decalagePixelVertical,cv::Range &decoupageLigne2, cv::Range &decoupageColonne2 , cv::Mat curImg);
void ChangementPointDeVue(cv::Mat img, cv::Mat projMatrix, cv::Point3f headPosition, cv::Mat result);