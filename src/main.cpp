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
#define ENTER_KEY 13

VideoCapture   cap; //la vid�o
Mat curImg ; //! l'image courante
int key = 0; //! touche du clavier
Mat imgFen; //! l'image d�coup�e
Mat imgFen2; //! l'image d�coup�e avec mouvement de la tete
float distanceCameraPaysage = 0.5f; //! distance paysage cam�ra (en m)
int decalagePixelHorizontal = 0; //! d�calage horizontal entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
int decalagePixelVertical = 0; //! d�calage vertical entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
bool headDetectorCalibrated = false; //! True si la profondeur a été calibrée

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

	/*! Première étape: calibration de la caméra "face" */
	float dist_btw_eyes;
	cout << "Distance entre les deux yeux (cm): " << endl;
	cin >> dist_btw_eyes;
	setEyeDistance(dist_btw_eyes);
	cout << "Placez-vous à 50cm de la caméra, au centre de l'image, et appuyez sur c" << endl;

	bool calibrating = false;

	//! Boucle de calibration
	while( (key != KEY_ESCAPE) && (key != ENTER_KEY) ) {
		cap>>curImg;
		Point3d temp;
		// Affiche de l'image courante et dessin des yeux si ils sont détectés

		if ( (key == 'c') || calibrating ){
			if (key != 'e') {
				headDetectorCalibrated = calibrateDepth(curImg);

				if (headDetectorCalibrated) {
					cout << "Calibration effectuée. \n\t- Appuyer sur Entrée pour confirmer.\n\t- Appuyer sur c pour refaire la calibration" << endl;
					calibrating = false;
				}
				else {
					if (key == 'c') {
						cout << "Calibration (appuyer sur e pour annuler)..." << endl;
						calibrating = true;
					}
				}
			}
			else {
				cout << "Calibration annulée. Repositionnez-vous et appuyez sur c" << endl;
				detectEyes(curImg, &temp, 1.0, true);
				calibrating = false;
			}
		}
		else {
			detectEyes(curImg, &temp, 1.0, true);
		}

		
		imshow(windowNameCapture, curImg);

		key = waitKey(1);
	}
	if (!headDetectorCalibrated)
	{	
		cout << "Calibration non effectuée. Les paramètres par défaut seront utilisés." << endl;
	}
	else {
		cout << "Calibration validée" << endl;
	}

	key = 'a';

	//! Boucle principale
	while(key!= KEY_ESCAPE){

		cap>>curImg;
		imshow(windowNameCapture, curImg);

		// Analyse de l'image et détection des yeux
		Point3d relative_pos;

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
