// inclusion des bibliothèques, dont OpenCV

#include <string.h>


#include <vector>
#include "deplacementImage.h"

// namespace
using namespace std;
using namespace cv;



// Default wdth and height of the video
#define DEFAULT_VIDEO_WIDTH   800
#define DEFAULT_VIDEO_HEIGHT  600
#define pixelParMetre 0.000311 ///http://www.yourwebsite.fr/index.php/documents/287-relation-entre-pixel-et-taille-s-des-images
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



	//! on récupère la largeur et hauteur de l'image
	int widthFrame  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int heightFrame = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	std::cout<<"Frame width = "<<widthFrame<<std::endl;
	std::cout<<"Frame height = "<<heightFrame<<std::endl;


	//! création des fenetres OpenCV
	//! Ce que renvoit la caméra
	string windowNameCapture = "Image obtenue par la caméra";
	namedWindow(windowNameCapture, CV_WINDOW_AUTOSIZE);
	imshow(windowNameCapture, curImg);

	//! L'image découpée effet fenêtre
	string windowNameCapture2 = "Fenêtre";
	namedWindow(windowNameCapture2, CV_WINDOW_AUTOSIZE);
	imshow(windowNameCapture2, curImg);
	
	//! L'image découpée effet fenêtre
	string windowNameCapture3 = "Fenêtre avec mouvement de la tête";
	namedWindow(windowNameCapture3, CV_WINDOW_AUTOSIZE);
	imshow(windowNameCapture3, curImg);


	//! on chosit la fenetre qu'on veut (ici un peu au hasard)
	Range decoupageColonne(heightFrame/2-150, heightFrame/2+150);
	Range decoupageLigne(widthFrame/2-100,widthFrame/2+100);
	

	float compteur=0;
	//! on récupère la position de la tête relative a la camera (ici elle est fixée) 
	float positionTete[3];
	positionTete[0]= 0;
	positionTete[1]= 0.5;
	positionTete[2]= 0;



	//! on l'affiche en boucle !
	while(key!= KEY_ESCAPE){

		compteur+=0.1;
		cap >> curImg;
		imshow(windowNameCapture, curImg);


		//! on coupe l'image pour qu'elle soit de la taille de la fenetre (position de la caméra en haut à droite de l'écran)
		imgFen = curImg(decoupageLigne,decoupageColonne);

		
		//! on l'affiche
		imshow(windowNameCapture2, imgFen);


		//!\\POUR PAUL : INSERER ICI LE CALCUL DE LA POSITION DE LA TETE//!\\

		//positionTete=...

	//! on change l'image obtenue
	//! d'après de longs calculs (thalès), l'image obtenue dépend de la distance de la caméra au "paysage"...
	//! On va donc récupérer la taille du marqueur à chaque frame, le comparer a sa taille réelle et en déduire ma distance caméra paysage
	//! Pour l'instant sans marqueur on suppose la distance = 50 cm

		

		calculDecalageFenetre(positionTete, decalagePixelVertical, decalagePixelHorizontal, distanceCameraPaysage, pixelParMetre);
		
//		appuiTouche(decalagePixelVertical, decalagePixelHorizontal);  Pour le debogage

		cv::Range decoupageLigne2;
		cv::Range decoupageColonne2;

		if (decoupageImage(decoupageLigne, decoupageColonne, decalagePixelHorizontal, decalagePixelVertical , decoupageColonne2, decoupageLigne2 , curImg))
		{
		//! Si c'est possible on coupe l'image et on l'affiche
		imgFen2 = * new Mat(heightFrame,widthFrame, 0);
		imgFen2 = curImg(decoupageLigne2,decoupageColonne2);
		imshow(windowNameCapture3, imgFen2);

		}


		

		key = waitKey(1);
	}

	cv::destroyAllWindows();

	return 0;
}














