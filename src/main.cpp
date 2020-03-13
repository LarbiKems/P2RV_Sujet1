#define NB_CAM_AVAILABLE 2
// inclusion des biblioth�ques, dont OpenCV

#include <string.h>

#include <vector>

// Inclusion des fichiers utiles à la reconnaissance de visage
#include "faceDetection.hpp"
#include "deplacementImage.h"
#include "CalibrationCamera.h"
#include "keyboard.h"

// namespace
using namespace std;
using namespace cv;

// Default wdth and height of the video
#define DEFAULT_VIDEO_WIDTH 800
#define DEFAULT_VIDEO_HEIGHT 600
#define pixelParMetre 0.000311 ///http://www.yourwebsite.fr/index.php/documents/287-relation-entre-pixel-et-taille-s-des-images



VideoCapture faceCamera; // Caméra frontale
VideoCapture mainCamera; // Caméra principale (caméra d'intêret)
Mat currentFaceImage;	//! l'image courante de la caméra frontale
Mat currentMainImage;	//! l'image courante de la caméra avant

int key = 0;						 //! touche du clavier
Mat finalImage;						 //! l'image d�coup�e avec mouvement de la tete
float distanceCameraPaysage = 0.5f;  //! distance paysage cam�ra (en m)
int decalagePixelHorizontal = 0;	 //! d�calage horizontal entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
int decalagePixelVertical = 0;		 //! d�calage vertical entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
Mat intrinsicCam;					     //! Matrice des paramètres intrinsèques de la caméra
int main()
{
	init_faceDetection();

	//! on r�cup�re l'image de la cam�ra avant dans une matrice
	//on r�cup�re l'image
	faceCamera.open(0);
	if (!faceCamera.isOpened())
	{
		cerr << "Erreur lors de l'initialisation de la capture de la camera !" << endl;
		cerr << "Fermeture..." << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		// on joue la premi�re image
		faceCamera >> currentFaceImage;
	}

#if (NB_CAM_AVAILABLE == 2)
	int camNb = 1;
	do
	{
		cout << "Caméra n° " << camNb << endl;
		mainCamera.open(camNb);
		camNb++;
	} while (!mainCamera.isOpened() && camNb < 10);

	if (!mainCamera.isOpened())
	{
		cerr << "Erreur lors de l'initialisation de la caméra mainCamera !" << endl;
		cerr << "Fermeture..." << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		// on joue la première image
		mainCamera >> currentMainImage;
	}
	//! on r�cup�re la largeur et hauteur de l'image
	int widthFrame = mainCamera.get(CV_CAP_PROP_FRAME_WIDTH);
	int heightFrame = mainCamera.get(CV_CAP_PROP_FRAME_HEIGHT);
#else
	currentMainImage = imread("lenna.png");
	if (currentMainImage.empty())
	{
		currentMainImage = imread("../lenna.png");
		if (currentMainImage.empty())
		{
			cerr << "Erreur dans la lecture de Lenna" << endl;
			exit(EXIT_FAILURE);
		}
	}
	//! on r�cup�re la largeur et hauteur de l'image
	int widthFrame = currentMainImage.rows;
	int heightFrame = currentMainImage.cols;
#endif

	//! on affiche la largeur et hauteur de l'image
	std::cout << "Frame width = " << widthFrame << std::endl;
	std::cout << "Frame height = " << heightFrame << std::endl;

	

	//! on chosit la fenetre qu'on veut (ici un peu au hasard)
	Range decoupageColonne(heightFrame / 2 - 150, heightFrame / 2 + 150);
	Range decoupageLigne(widthFrame / 2 - 100, widthFrame / 2 + 100);


	/*! Calibration de la caméra et récupération de la matrice intrinsèque */
	intrinsicCam = calibrateCamera(mainCamera);
	calibrateFaceCamera(faceCamera);

	//! création des fenetres OpenCV

	//! Ce que renvoit la caméra
	string mainImageName = "Image obtenue par la camera";
	namedWindow(mainImageName, CV_WINDOW_AUTOSIZE);
	imshow(mainImageName, currentMainImage);

	//! L'image d�coup�e effet fen�tre
	string finalImageName = "Resultat";
	namedWindow(finalImageName, CV_WINDOW_AUTOSIZE);
	//imshow(finalImageName, currentFaceImage);

	string faceImageName = "Image apres detection de visage";
	namedWindow(faceImageName, CV_WINDOW_AUTOSIZE);
	imshow(faceImageName, currentFaceImage);

	key = 'a';

	//! Boucle principale
	Point3f relative_pos(0, 0, 0);
	
	while (key != KEY_ESCAPE)
	{
		//! Récupération de l'image de la caméra frontale
		faceCamera >> currentFaceImage;
		flip(currentFaceImage, currentFaceImage, 1); // Effet mirroir

		//! Récupération de l'image de la caméra principale
#if (NB_CAM_AVAILABLE == 2)
		mainCamera >> currentMainImage;
#endif

		// Analyse de l'image et détection des yeux
		// relative_pos est mis à jour si les yeux sont détectés
		bool detected = detectEyes(currentFaceImage, &relative_pos, 1, true, true);
		cout << relative_pos << endl;

		//! Modification du point de vu à partir de la position de la tête
		ChangementPointDeVue(currentMainImage, intrinsicCam,relative_pos, finalImage);

		// Affichage des images
		imshow(finalImageName, finalImage);
		imshow(faceImageName, currentFaceImage);
		imshow(mainImageName, currentMainImage);

		key = waitKey(1);
	}

	cv::destroyAllWindows();

	return 0;
}
