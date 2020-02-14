// inclusion des biblioth�ques, dont OpenCV

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



	//! on r�cup�re la largeur et hauteur de l'image
	int widthFrame  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int heightFrame = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	std::cout<<"Frame width = "<<widthFrame<<std::endl;
	std::cout<<"Frame height = "<<heightFrame<<std::endl;


	//! cr�ation des fenetres OpenCV
	//! Ce que renvoit la cam�ra
	string windowNameCapture = "Image obtenue par la cam�ra";
	namedWindow(windowNameCapture, CV_WINDOW_AUTOSIZE);
	imshow(windowNameCapture, curImg);

	//! L'image d�coup�e effet fen�tre
	string windowNameCapture2 = "Fen�tre";
	namedWindow(windowNameCapture2, CV_WINDOW_AUTOSIZE);
	imshow(windowNameCapture2, curImg);
	
	//! L'image d�coup�e effet fen�tre
	string windowNameCapture3 = "Fen�tre avec mouvement de la t�te";
	namedWindow(windowNameCapture3, CV_WINDOW_AUTOSIZE);
	imshow(windowNameCapture3, curImg);


	//! on chosit la fenetre qu'on veut (ici un peu au hasard)
	Range decoupageColonne(heightFrame/2-150, heightFrame/2+150);
	Range decoupageLigne(widthFrame/2-100,widthFrame/2+100);
	

	float compteur=0;
	//! on r�cup�re la position de la t�te relative a la camera (ici elle est fix�e) 
	float positionTete[3];
	positionTete[0]= 0;
	positionTete[1]= 0.5;
	positionTete[2]= 0;



	//! on l'affiche en boucle !
	while(key!= KEY_ESCAPE){

		compteur+=0.1;
		cap >> curImg;
		imshow(windowNameCapture, curImg);


		//! on coupe l'image pour qu'elle soit de la taille de la fenetre (position de la cam�ra en haut � droite de l'�cran)
		imgFen = curImg(decoupageLigne,decoupageColonne);

		
		//! on l'affiche
		imshow(windowNameCapture2, imgFen);


		//!\\POUR PAUL : INSERER ICI LE CALCUL DE LA POSITION DE LA TETE//!\\

		//positionTete=...

	//! on change l'image obtenue
	//! d'apr�s de longs calculs (thal�s), l'image obtenue d�pend de la distance de la cam�ra au "paysage"...
	//! On va donc r�cup�rer la taille du marqueur � chaque frame, le comparer a sa taille r�elle et en d�duire ma distance cam�ra paysage
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














