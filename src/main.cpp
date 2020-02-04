// inclusion des biblioth�ques, dont OpenCV

#include <string.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>

// Inclusion des fichiers utiles à la reconnaissance de visage
#include "faceDetection.hpp"

// namespace
using namespace std;
using namespace cv;


// Default wdth and height of the video
#define DEFAULT_VIDEO_WIDTH   800
#define DEFAULT_VIDEO_HEIGHT  600
#define pixelParMetre 0.000264 ///http://www.yourwebsite.fr/index.php/documents/287-relation-entre-pixel-et-taille-s-des-images
// Defining escape key
#define KEY_ESCAPE 27

VideoCapture   cap; //la vid�o
Mat curImg ; //! l'image courante
int key = 0; //! touche du clavier
Mat imgFen; //! l'image d�coup�e
Mat imgFen2; //! l'image d�coup�e avec mouvement de la tete
float distanceCameraPaysage = 0.5f; //! distance paysage cam�ra (en m)
int decalagePixelHorizontal = 0; //! d�calage horizontal entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
int decalagePixelVertical = 0; //! d�calage vertical entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te


int main(){


	//! on r�cup�re l'image de la cam�ra avant dans une matrice
	//on r�cup�re l'image
	cap.open(0);
	if(!cap.isOpened()) {
		cerr << "Erreur lors de l'initialisation de la capture de la camera !"<< endl;
		cerr << "Fermeture..." << endl;
		exit(EXIT_FAILURE);
	}
	else{
		// on joue la premi�re image
		cap >> curImg;
	}
	//! cr�ation des fenetres OpenCV
	//! Ce que renvoit la cam�ra
	string windowNameCapture = "Image obtenue par la caméra";
	namedWindow(windowNameCapture, CV_WINDOW_AUTOSIZE);

  string windowNameVisage = "Image après détection de visage";
  namedWindow(windowNameVisage, CV_WINDOW_AUTOSIZE);


	//! on l'affiche en boucle !
	while(key!= KEY_ESCAPE){

		cap>>curImg;
		imshow(windowNameCapture, curImg);

    // Analyse de l'image et détection des yeux
	Point relative_pos;

	if (detectEyes(curImg, &relative_pos, 1, true, true)) {
		// Affichage de la position des yeux par rapport au centre de l'image
		std::cout << relative_pos << std::endl;
	}   
    imshow(windowNameVisage, curImg);

		//! on r�cup�re la position de la t�te relative a la camera (ici elle est fix�e)

		//float positionTete[3]=...



		key = waitKey(1);
	}

	cv::destroyAllWindows();

	return 0;
}
