#define NB_CAM_AVAILABLE 1
// inclusion des biblioth�ques, dont OpenCV

#include <string.h>

#include <vector>

// Inclusion des fichiers utiles à la reconnaissance de visage
#include "faceDetection.hpp"
#include "deplacementImage.h"
#include "CalibrationCamera.h"

// namespace
using namespace std;
using namespace cv;

// Default wdth and height of the video
#define DEFAULT_VIDEO_WIDTH 800
#define DEFAULT_VIDEO_HEIGHT 600
#define pixelParMetre 0.000311 ///http://www.yourwebsite.fr/index.php/documents/287-relation-entre-pixel-et-taille-s-des-images
// Defining escape key

#define ENTER_KEY 13

char key;

VideoCapture faceCamera; // Caméra frontale
VideoCapture mainCamera; // Caméra principale (caméra d'intêret)
Mat currentFaceImage;	//! l'image courante de la caméra frontale
Mat currentMainImage;	//! l'image courante de la caméra avant

int key = 0;						 //! touche du clavier
Mat imgFen;							 //! l'image d�coup�e
Mat imgFen2;						 //! l'image d�coup�e avec mouvement de la tete
float distanceCameraPaysage = 0.5f;  //! distance paysage cam�ra (en m)
int decalagePixelHorizontal = 0;	 //! d�calage horizontal entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
int decalagePixelVertical = 0;		 //! d�calage vertical entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
bool headDetectorCalibrated = false; //! True si la profondeur a été calibrée
Mat intrinsicCam;					     //! Matrice des paramètres intrinsèques de la caméra
int main()
{

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

	//! création des fenetres OpenCV

	//! Ce que renvoit la caméra
	string mainImageName = "Image obtenue par la caméra";
	namedWindow(mainImageName, CV_WINDOW_AUTOSIZE);
	imshow(mainImageName, currentMainImage);

	//! L'image d�coup�e effet fen�tre
	string mainImageCutName = "Image obtenue par la caméra, coupée";
	namedWindow(mainImageCutName, CV_WINDOW_AUTOSIZE);
	//imshow(mainImageCutName, currentFaceImage);

	//! L'image d�coup�e effet fen�tre
	string mainImageAfterTreatmentName = "Résultat";
	namedWindow(mainImageAfterTreatmentName, CV_WINDOW_AUTOSIZE);
	//imshow(mainImageAfterTreatmentName, currentFaceImage);

	string faceImageName = "Image après détection de visage";
	namedWindow(faceImageName, CV_WINDOW_AUTOSIZE);
	imshow(faceImageName, currentFaceImage);

	//! on chosit la fenetre qu'on veut (ici un peu au hasard)
	Range decoupageColonne(heightFrame / 2 - 150, heightFrame / 2 + 150);
	Range decoupageLigne(widthFrame / 2 - 100, widthFrame / 2 + 100);


	/*! Calibration de la caméra et récupération de la matrice intrinsèque */

	intrinsicCam = calibrateCamera(mainCamera);
	



	/*! Première étape: calibration de la caméra "face" */
	float dist_btw_eyes;
	cout << "Distance entre les deux yeux (cm): " << endl;
	cin >> dist_btw_eyes;
	setEyeDistance(dist_btw_eyes);
	cout << "Placez-vous à 50cm de la caméra, au centre de l'image, et appuyez sur c" << endl;


	bool calibrating = false;

	//! Boucle de calibration
	while ((key != KEY_ESCAPE) && (key != ENTER_KEY))
	{
		faceCamera >> currentFaceImage;
		Point3f temp;
		// Affiche de l'image courante et dessin des yeux si ils sont détectés

		if ((key == 'c') || calibrating)
		{
			if (key != 'e')
			{
				headDetectorCalibrated = calibrateDepth(currentFaceImage);

				if (headDetectorCalibrated)
				{
					cout << "Calibration effectuée. \n\t- Appuyer sur Entrée pour confirmer.\n\t- Appuyer sur c pour refaire la calibration" << endl;
					calibrating = false;
				}
				else
				{
					if (key == 'c')
					{
						cout << "Calibration (appuyer sur e pour annuler)..." << endl;
						calibrating = true;
					}
				}
			}
			else
			{
				cout << "Calibration annulée. Repositionnez-vous et appuyez sur c" << endl;
				detectEyes(currentFaceImage, &temp, 1.0, true);
				calibrating = false;
			}
		}
		else
		{
			detectEyes(currentFaceImage, &temp, 1.0, true);
		}

		imshow(faceImageName, currentFaceImage);

		key = waitKey(1);
	}

	if (!headDetectorCalibrated)
	{
		cout << "Calibration non effectuée. Les paramètres par défaut seront utilisés." << endl;
	}
	else
	{
		cout << "Calibration validée" << endl;
	}

	key = 'a';

	//! Boucle principale
	Point3f relative_pos(0, 0, 0);

	imgFen2 = currentMainImage(decoupageLigne, decoupageColonne);
	
	while (key != KEY_ESCAPE)
	{

		faceCamera >> currentFaceImage;
#if (NB_CAM_AVAILABLE == 2)
		mainCamera >> currentMainImage;
#endif

		// Analyse de l'image et détection des yeux
		// relative_pos est mis à jour si les yeux sont détectés
		detectEyes(currentFaceImage, &relative_pos, 1, true, true);
		cout << relative_pos << endl;

		// ! on coupe l'image pour qu'elle soit de la taille de la fenetre (position de la cam�ra en haut � droite de l'�cran)
		imgFen = currentMainImage(decoupageLigne, decoupageColonne);

		//! on change l'image obtenue
		//! d'apr�s de longs calculs (thal�s), l'image obtenue d�pend de la distance de la cam�ra au "paysage"...
		//! On va donc r�cup�rer la taille du marqueur � chaque frame, le comparer a sa taille r�elle et en d�duire ma distance cam�ra paysage
		//! Pour l'instant sans marqueur on suppose la distance = 50 cm

		calculDecalageFenetre(relative_pos, decalagePixelVertical, decalagePixelHorizontal, distanceCameraPaysage, pixelParMetre);

		cv::Range decoupageLigne2;
		cv::Range decoupageColonne2;

		if (decoupageImage(decoupageLigne, decoupageColonne, decalagePixelHorizontal, decalagePixelVertical, decoupageColonne2, decoupageLigne2, currentMainImage))
		{
			//! Si c'est possible on coupe l'image et on l'affiche
			imgFen2 = *new Mat(heightFrame, widthFrame, 0);
			imgFen2 = currentMainImage(decoupageLigne2, decoupageColonne2);
			cout << "DANS le champs" << endl;
		}
		else
		{
			cout << "L'image est sortie du champs" << endl;
			//imgFen2 = imgFen;
		}

		// Affichage des images
		imshow(mainImageAfterTreatmentName, imgFen2);
		imshow(mainImageCutName, imgFen);
		imshow(faceImageName, currentFaceImage);
		imshow(mainImageName, currentMainImage);

		key = waitKey(1);
	}

	cv::destroyAllWindows();

	return 0;
}
