/*! Première Version du Projet. Elle contient un programme fonctionnel : un déplacement a droite de la tête 
 *  induit bien un déplacement a gauche de l'image. Cependant, on preferera utiliser des matrices de 
 *  projections pour obtenir un résultat plus réaliste.
 */

// inclusion des bibliothèques
#include <string.h>
#include <vector>

#include "faceDetection.hpp"
#include "deplacementImage.h"

// Touches du clavier
#include "keys.h"

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

	//! on choisit la fenetre que l'on veut découper dans l'image de mainCamera
	Range decoupageColonne(heightFrame / 2 - 150, heightFrame / 2 + 150);
	Range decoupageLigne(widthFrame / 2 - 100, widthFrame / 2 + 100);

	/*! Calibration de la caméra avant*/
	calibrateFaceCamera(faceCamera);

	/*! création des fenetres OpenCV */

	//! Ce que renvoit la caméra
	string mainImageName = "Image obtenue par la caméra";
	namedWindow(mainImageName, CV_WINDOW_AUTOSIZE);

	//! L'image découpée effet fenètre
	string mainImageCutName = "Image obtenue par la caméra, coupée";
	namedWindow(mainImageCutName, CV_WINDOW_AUTOSIZE);

	//! L'image d�coup�e effet fen�tre
	string mainImageAfterTreatmentName = "Résultat";
	namedWindow(mainImageAfterTreatmentName, CV_WINDOW_AUTOSIZE);

	string faceImageName = "Image après détection de visage";
	namedWindow(faceImageName, CV_WINDOW_AUTOSIZE);

	/*! Boucle principale */
	int key = 0;
	Point3f relative_pos(0, 0, 0);
	Mat resultImage = currentMainImage(decoupageLigne, decoupageColonne);
	int decalagePixelHorizontal = 0; //! d�calage horizontal entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te
	int decalagePixelVertical = 0;	 //! d�calage vertical entre l'image obtenue par effet fen�tre et celle avec mouvement de la t�te

	while (key != KEY_ESCAPE)
	{

		/*! Récupération des images */

		faceCamera >> currentFaceImage; // Récupération de l'image de face
		if (nb_cam == 2)
		{
			mainCamera >> currentMainImage; // Récupération de l'image principale
		}

		/*! Traitement de l'image de face */

		// Effet mirroir
		flip(currentFaceImage, currentFaceImage, 1);

		// Analyse de l'image et détection des yeux
		// relative_pos est mis à jour si les yeux sont détectés
		bool detected = detectEyes(currentFaceImage, &relative_pos, 1, true, true);

		/*! Création de l'image finale */

		// Calculs du de la position de la fenêtre dans l'image en fonction de la
		// position de la tête calculée précedemment
		calculDecalageFenetre(relative_pos, decalagePixelVertical, decalagePixelHorizontal);

		cv::Range decoupageLigne2;
		cv::Range decoupageColonne2;

		//! Si la fenêtre choisie ne sort pas du champ, on l'extrait et la stocke dans resultImage
		if (decoupageImage(decoupageLigne, decoupageColonne, decalagePixelHorizontal, decalagePixelVertical, decoupageLigne2, decoupageColonne2, currentMainImage))
		{
			resultImage = currentMainImage(decoupageLigne2, decoupageColonne2);
		}

		/*! Affichage des images */
		imshow(mainImageAfterTreatmentName, resultImage);
		imshow(faceImageName, currentFaceImage);
		imshow(mainImageName, currentMainImage);

		key = waitKey(1);
	}

	cv::destroyAllWindows();

	return 0;
}
