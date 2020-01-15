// inclusion des bibliothèques, dont OpenCV

#include <string.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
// namespace
using namespace std;
using namespace cv;



// Default wdth and height of the video
#define DEFAULT_VIDEO_WIDTH   800
#define DEFAULT_VIDEO_HEIGHT  600
#define pixelParMetre 0.000264 ///http://www.yourwebsite.fr/index.php/documents/287-relation-entre-pixel-et-taille-s-des-images
// Defining escape key
#define KEY_ESCAPE 27

VideoCapture   cap; //la vidéo
Mat curImg ; //! l'image courante
int key = 0; //! touche du clavier
Mat imgFen; //! l'image découpée
Mat imgFen2; //! l'image découpée avec mouvement de la tete
float distanceCameraPaysage = 0.5f; //! distance paysage caméra (en m)
int decalagePixelHorizontal = 0; //! décalage horizontal entre l'image obtenue par effet fenêtre et celle avec mouvement de la tête
int decalagePixelVertical = 0; //! décalage vertical entre l'image obtenue par effet fenêtre et celle avec mouvement de la tête


int main(){


	//! on récupère l'image de la caméra avant dans une matrice
	//on récupère l'image
	cap.open(0);
	if(!cap.isOpened()) {
		cerr << "Erreur lors de l'initialisation de la capture de la camera !"<< endl;
		cerr << "Fermeture..." << endl;
		exit(EXIT_FAILURE);
	}
	else{
		// on joue la première image
		cap >> curImg;
	}
	//! création des fenetres OpenCV
	//! Ce que renvoit la caméra
	string windowNameCapture = "Image obtenue par la caméra";
	namedWindow(windowNameCapture, CV_WINDOW_AUTOSIZE);
	imshow(windowNameCapture, curImg);
	

	//! on l'affiche en boucle !
	while(key!= KEY_ESCAPE){

		cap>>curImg;
		imshow(windowNameCapture, curImg);



		//! on récupère la position de la tête relative a la camera (ici elle est fixée) 
		
		//float positionTete[3]=...
		
		

		key = waitKey(1);
	}

	cv::destroyAllWindows();

	return 0;
}














