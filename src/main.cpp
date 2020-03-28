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

int main()
{
	
	VideoCapture faceCamera; // Caméra frontale
	VideoCapture mainCamera; // Caméra principale (caméra d'intêret)

	// Matrices qui stockent les images des caméras
	Mat currentFaceImage; //! l'image courante de la caméra frontale
	Mat currentMainImage; //! l'image courante de la caméra avant

	// Demande du nombre de caméras disponibles
	int nb_cam = -1;
	cout << "Nombre de caméras branchées: ";
	cin >> nb_cam;
	if (nb_cam < 1)
	{
		cerr << "L'application nécessite au moins 1 caméra" << endl;
		exit(EXIT_FAILURE);
	}

	//! Ouverture de la caméra faceCamera
	faceCamera.open(0);
	if (!faceCamera.isOpened())
	{
		cerr << "Erreur lors de l'initialisation de la caméra faceCamera !" << endl;
		cerr << "Fermeture..." << endl;
		exit(EXIT_FAILURE);
	}

	int widthFrame;
	int heightFrame;
	if (nb_cam == 2)
	{
		// Recherche de la seconde caméra. Cette boucle est réalisée car sur Linux, la première caméra est 0, mais la seconde peut être 1, 2, ..., 7, 8.
		int camNb = 1;
		do
		{
			mainCamera.open(camNb);
			camNb++;
		} while (!mainCamera.isOpened() && camNb < 15);

		if (!mainCamera.isOpened())
		{
			cerr << "Erreur lors de l'initialisation de la caméra mainCamera !" << endl;
			cerr << "Fermeture..." << endl;
			exit(EXIT_FAILURE);
		}

		//! on récupère la largeur et hauteur de l'image
		widthFrame = mainCamera.get(CV_CAP_PROP_FRAME_WIDTH);
		heightFrame = mainCamera.get(CV_CAP_PROP_FRAME_HEIGHT);

		mainCamera >> currentMainImage;
	}
	else
	{
		// Si une seule caméra est à disposition, l'image de mainCamera sera la photo de Lenna.
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
		//! on récupère la largeur et la hauteur de l'image
		widthFrame = currentMainImage.rows;
		heightFrame = currentMainImage.cols;
	}

	//! on affiche la largeur et la hauteur de l'image
	std::cout << "Frame width = " << widthFrame << std::endl;
	std::cout << "Frame height = " << heightFrame << std::endl;

	//! on chosit la fenetre qu'on veut (ici un peu au hasard)
	Range decoupageColonne(heightFrame / 2 - 150, heightFrame / 2 + 150);
	Range decoupageLigne(widthFrame / 2 - 100, widthFrame / 2 + 100);

	/*! Calibration de la caméra et récupération de la matrice intrinsèque */
	Mat intrinsicCam = calibrateMainCamera(mainCamera, nb_cam);
	init_faceDetection();
	calibrateFaceCamera(faceCamera);

	//! création des fenetres OpenCV

	//! Ce que renvoit la caméra
	string mainImageName = "Image obtenue par la camera";
	namedWindow(mainImageName, CV_WINDOW_AUTOSIZE);

	//! L'image d�coup�e effet fen�tre
	string finalImageName = "Resultat";
	namedWindow(finalImageName, CV_WINDOW_AUTOSIZE);

	string faceImageName = "Image apres detection de visage";
	namedWindow(faceImageName, CV_WINDOW_AUTOSIZE);

	
	//! Boucle principale
	int key = 0;
	Mat finalImage = *(new Mat(currentMainImage.clone()));						//! l'image d�coup�e avec mouvement de la tete
	Point3f relative_pos(0, 0, 0);
	
	while (key != KEY_ESCAPE)
	{
		//! Récupération de l'image de la caméra frontale
		faceCamera >> currentFaceImage;
		flip(currentFaceImage, currentFaceImage, 1); // Effet mirroir

		//! Récupération de l'image de la caméra principale
		if (nb_cam == 2)
		{
			mainCamera >> currentMainImage;
		}

		// Analyse de l'image et détection des yeux
		// relative_pos est mis à jour si les yeux sont détectés
		bool detected = detectEyes(currentFaceImage, &relative_pos, 1, true, true);
		cout << relative_pos << endl;
		
		//! Modification du point de vu à partir de la position de la tête
		ChangementPointDeVue(currentMainImage, intrinsicCam, relative_pos, finalImage);
		// Affichage des images
		imshow(finalImageName, finalImage);
		imshow(faceImageName, currentFaceImage);
		imshow(mainImageName, currentMainImage);

		key = waitKey(1);
	}

	cv::destroyAllWindows();

	return 0;
}
